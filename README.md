# People Counter with IR Limit (Arduino Uno + IR x2 + Relay Active-LOW + Buzzer + I2C LCD 16x2)

ระบบนับจำนวนคนเข้า–ออกด้วย IR 2 จุด (ลำดับ IR1->IR2 = เข้า, IR2->IR1 = ออก) แสดงผลบน LCD 16x2 (I2C) และควบคุมไฟ 12V ผ่านรีเลย์

- เมื่อจำนวนคน <= 40: ไฟเขียวติด
- เมื่อจำนวนคน > 40: ไฟแดงติด + Buzzer เตือนเป็นจังหวะ

หมายเหตุ: โปรเจกต์นี้ตั้งค่าให้รีเลย์เป็น Active-LOW โดยค่าเริ่มต้น (สั่งติดด้วย LOW, ดับด้วย HIGH)

---

## Features
- นับเข้า/ออกด้วย Finite State Machine (FSM) พร้อม debounce และ timeout
- ตั้งค่า CAP_LIMIT ได้ (ค่าเริ่มต้น 40)
- ควบคุมไฟสถานะ 12V (เขียว/แดง) ผ่านรีเลย์ 2 ช่อง
- Buzzer 5V เตือนเมื่อเกินความจุ
- LCD I2C 16x2 แสดง Count/Limit และสถานะ OK / FULL! พร้อมลูกศรชั่วครู่บอกทิศทาง

---

## Bill of Materials
- Arduino Uno (R3 หรือเทียบเท่า)
- IR Sensor (เอาต์พุตดิจิทัล DO) x 2
- Relay Module 5V x 2 ช่อง (Active-LOW)
- Buzzer 5V (Active)
- หลอดไฟ 12V สีเขียว x 1, สีแดง x 1
- LCD 16x2 แบบ I2C (ที่อยู่ทั่วไป 0x27 หรือ 0x3F)
- สายไฟ/คอนเนคเตอร์ และแหล่งจ่าย 12V สำหรับหลอดไฟ

---

## Wiring

### I2C LCD 16x2 -> Arduino Uno
| LCD | Uno |
| --- | --- |
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

### Sensors/Control -> Arduino Uno
| อุปกรณ์ | ขาโมดูล | ขา Uno |
| --- | --- | --- |
| IR1 (ด้านนอก) | DO | D2 |
| IR2 (ด้านใน)  | DO | D3 |
| Buzzer 5V (Active) | + | D9 |
| Relay CH1 (ไฟเขียว) | IN1 | D6 |
| Relay CH2 (ไฟแดง)   | IN2 | D7 |
| IR Modules | VCC/GND | 5V/GND |
| Relay Module | VCC/GND | 5V/GND |

สำคัญ: ต่อ GND ร่วมระหว่าง Arduino, IR และรีเลย์

### Relay -> หลอดไฟ 12V
- CH1 (ไฟเขียว): 12V(+) -> COM1 ; NO1 -> (+) หลอดเขียว ; (-) หลอดเขียว -> 12V(-)
- CH2 (ไฟแดง): 12V(+) -> COM2 ; NO2 -> (+) หลอดแดง ; (-) หลอดแดง -> 12V(-)

เส้น 12V ผ่านหน้าสัมผัสรีเลย์โดยตรง ไม่ต้องเชื่อมกับ 5V ของ Arduino

---

## Folder Structure (suggested)
