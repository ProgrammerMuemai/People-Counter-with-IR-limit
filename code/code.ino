// People Counter with 2 IR sensors + Relay (Green/Red 12V) + Buzzer (5V) + I2C LCD 16x2
// Board: Arduino Uno

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======= LCD Config (แก้ address ได้ถ้าไม่ใช่ 0x27) =======
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======= Pin Mapping =======
const byte PIN_IR1   = 2;   // IR ด้านนอก
const byte PIN_IR2   = 3;   // IR ด้านใน
const byte PIN_BUZZ  = 9;   // Buzzer Active 5V
const byte PIN_RELAY_GREEN = 6; // Relay CH1 -> ไฟเขียว 12V
const byte PIN_RELAY_RED   = 7; // Relay CH2 -> ไฟแดง 12V

// ======= Behavior Config =======
const int  CAP_LIMIT = 40;           // เกณฑ์ความจุ
const bool RELAY_ACTIVE_LOW = true;  // รีเลย์ Active-LOW
const unsigned long DEBOUNCE_MS = 50;
const unsigned long STEP_TIMEOUT_MS = 1500;
const unsigned long BUZZ_ON_MS  = 200;
const unsigned long BUZZ_OFF_MS = 800;

// ======= Internal State =======
enum StepState {
  IDLE = 0,
  WAIT_IR2_AFTER_IR1,  // คาดว่าจะ "เข้า" (IR1 -> IR2)
  WAIT_IR1_AFTER_IR2   // คาดว่าจะ "ออก" (IR2 -> IR1)
};

StepState state = IDLE;
unsigned long stepStartMs = 0;
unsigned long lastChangeIR1 = 0;
unsigned long lastChangeIR2 = 0;
int peopleCount = 0;

// สำหรับ Buzzer จังหวะกระพริบ
unsigned long buzzTimer = 0;
bool buzzOnPhase = false;

// สำหรับ LCD เพื่อลดการเขียนซ้ำโดยไม่จำเป็น
int lastShownCount = -1;
bool lastShownOver = false;

// ======= Helper: อ่านสัญญาณ IR =======
// โมดูล IR ทั่วไป: ถูกบัง -> DO = LOW
bool isIR1Blocked() { return digitalRead(PIN_IR1) == LOW; }
bool isIR2Blocked() { return digitalRead(PIN_IR2) == LOW; }

// เขียนรีเลย์เผื่อ Active LOW/HIGH
void setRelay(byte pin, bool on) {
  if (RELAY_ACTIVE_LOW) {
    digitalWrite(pin, on ? LOW : HIGH);
  } else {
    digitalWrite(pin, on ? HIGH : LOW);
  }
}

void updateLCD(bool enteringHint=false, bool exitingHint=false) {
  bool over = (peopleCount > CAP_LIMIT);
  if (peopleCount == lastShownCount && over == lastShownOver && !enteringHint && !exitingHint) return;

  lcd.clear();
  // บรรทัด 1: Count และ Limit
  lcd.setCursor(0, 0);
  lcd.print("Count:");
  // ให้แน่ใจว่าเว้นวรรคพอ
  lcd.setCursor(7, 0);
  char buf[16];
  snprintf(buf, sizeof(buf), "%2d/%-2d", peopleCount, CAP_LIMIT);
  lcd.print(buf);

  // บรรทัด 2: สถานะ
  lcd.setCursor(0, 1);
  if (over) {
    lcd.print("Status: FULL!   ");   // เกิน 40
  } else {
    lcd.print("Status: OK      ");   // ยังไม่เกิน
  }

  // แสดงลูกศรชั่วครู่เวลา detect ทิศทาง
  // (ใช้พื้นที่ท้ายบรรทัด 2)
  if (enteringHint) {
    lcd.setCursor(14, 1); lcd.print("->"); // เข้า
  } else if (exitingHint) {
    lcd.setCursor(14, 1); lcd.print("<-"); // ออก
  }

  lastShownCount = peopleCount;
  lastShownOver  = over;
}

void updateLightsAndBuzzer() {
  if (peopleCount <= CAP_LIMIT) {
    setRelay(PIN_RELAY_GREEN, true);
    setRelay(PIN_RELAY_RED,   false);
    noTone(PIN_BUZZ);
    buzzOnPhase = false;
  } else {
    setRelay(PIN_RELAY_GREEN, false);
    setRelay(PIN_RELAY_RED,   true);

    unsigned long now = millis();
    if (!buzzOnPhase) {
      if (now - buzzTimer >= BUZZ_OFF_MS) {
        tone(PIN_BUZZ, 2000); // 2kHz
        buzzTimer = now;
        buzzOnPhase = true;
      }
    } else {
      if (now - buzzTimer >= BUZZ_ON_MS) {
        noTone(PIN_BUZZ);
        buzzTimer = now;
        buzzOnPhase = false;
      }
    }
  }
}

void resetToIdle() {
  state = IDLE;
  stepStartMs = 0;
}

void setup() {
  pinMode(PIN_IR1, INPUT_PULLUP);
  pinMode(PIN_IR2, INPUT_PULLUP);
  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_RELAY_GREEN, OUTPUT);
  pinMode(PIN_RELAY_RED, OUTPUT);

  setRelay(PIN_RELAY_GREEN, false);
  setRelay(PIN_RELAY_RED, false);
  noTone(PIN_BUZZ);

  Serial.begin(115200);
  Serial.println(F("People Counter + LCD START"));

  // LCD init
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("People Counter");
  lcd.setCursor(0,1); lcd.print("Init...");
  delay(800);
  updateLCD(); // แสดงค่าตั้งต้น
}

void loop() {
  unsigned long now = millis();

  // อ่านสถานะ IR พร้อม debounce แบบง่าย
  static bool prevIR1 = !isIR1Blocked();
  static bool prevIR2 = !isIR2Blocked();
  bool curIR1 = isIR1Blocked();
  bool curIR2 = isIR2Blocked();

  bool hintEnter = false;
  bool hintExit  = false;

  // ตรวจจับ IR1
  if (curIR1 != prevIR1) {
    if (now - lastChangeIR1 >= DEBOUNCE_MS) {
      lastChangeIR1 = now;
      prevIR1 = curIR1;

      if (curIR1) { // IR1 ถูกบัง
        if (state == IDLE) {
          state = WAIT_IR2_AFTER_IR1; // เดา "เข้า"
          stepStartMs = now;
        } else if (state == WAIT_IR1_AFTER_IR2) {
          // ลำดับ IR2 -> IR1 ครบ => ออก
          peopleCount = max(0, peopleCount - 1);
          Serial.print(F("Exit | Count: ")); Serial.println(peopleCount);
          resetToIdle();
          hintExit = true;
        }
      }
    }
  }

  // ตรวจจับ IR2
  if (curIR2 != prevIR2) {
    if (now - lastChangeIR2 >= DEBOUNCE_MS) {
      lastChangeIR2 = now;
      prevIR2 = curIR2;

      if (curIR2) { // IR2 ถูกบัง
        if (state == IDLE) {
          state = WAIT_IR1_AFTER_IR2; // เดา "ออก"
          stepStartMs = now;
        } else if (state == WAIT_IR2_AFTER_IR1) {
          // ลำดับ IR1 -> IR2 ครบ => เข้า
          peopleCount++;
          Serial.print(F("Enter | Count: ")); Serial.println(peopleCount);
          resetToIdle();
          hintEnter = true;
        }
      }
    }
  }

  // หมดเวลา step -> รีเซ็ต
  if (state != IDLE && (now - stepStartMs > STEP_TIMEOUT_MS)) {
    resetToIdle();
  }

  // อัปเดตไฟ/เสียง + LCD
  updateLightsAndBuzzer();
  updateLCD(hintEnter, hintExit);

  delay(5);
}
