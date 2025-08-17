People Counter with IR Limit (Arduino Uno + IR x2 + Relay Active-LOW + Buzzer + I2C LCD 16×2)

ระบบนับจำนวนคนเข้า–ออกด้วย IR 2 จุด (ลำดับ IR1→IR2 = เข้า, IR2→IR1 = ออก) แสดงผลบน LCD 16×2 (I2C) และควบคุม ไฟ 12V ผ่านรีเลย์

เมื่อ ≤ 40 คน: ไฟ เขียว ติด

เมื่อ > 40 คน: ไฟ แดง ติด + Buzzer เตือนเป็นจังหวะ

โปรเจกต์นี้ตั้งค่าให้ รีเลย์เป็น Active-LOW โดยค่าเริ่มต้น (สั่งติดด้วย LOW, ดับด้วย HIGH)

📦 อุปกรณ์ที่ใช้

Arduino Uno (R3 หรือเทียบเท่า)

IR Sensor (เอาต์พุตดิจิทัล DO) × 2

Relay Module 5V × 2 ช่อง (Active-LOW)

Buzzer 5V (แบบ Active)

หลอดไฟ 12V สีเขียว ×1, 12V สีแดง ×1

LCD 16×2 (I2C) (ที่อยู่ทั่วไป 0x27 หรือ 0x3F)

สายไฟ/คอนเนคเตอร์ + แหล่งจ่าย 12V สำหรับหลอดไฟ

🔌 การเชื่อมต่อ
I2C LCD 16×2 → Arduino Uno
LCD	Uno
VCC	5V
GND	GND
SDA	A4
SCL	A5
สัญญาณควบคุม/เซ็นเซอร์ → Arduino Uno
อุปกรณ์	ขาโมดูล	ขา Uno
IR1 (ด้านนอก)	DO	D2
IR2 (ด้านใน)	DO	D3
Buzzer 5V (Active)	+	D9
Relay CH1 (ไฟเขียว)	IN1	D6
Relay CH2 (ไฟแดง)	IN2	D7
IR Modules	VCC/GND	5V/GND
Relay Module	VCC/GND	5V/GND

สำคัญ: ต่อ GND ร่วม ระหว่าง Arduino, IR และรีเลย์

รีเลย์ ↔ หลอดไฟ 12V

CH1 → ไฟเขียว: 12V(+) → COM1 ; NO1 → (+) หลอดเขียว ; (−) หลอดเขียว → 12V(−)

CH2 → ไฟแดง: 12V(+) → COM2 ; NO2 → (+) หลอดแดง ; (−) หลอดแดง → 12V(−)

เส้น 12V ผ่านหน้าสัมผัสรีเลย์โดยตรง ไม่ต้อง เชื่อมกับ 5V ของ Arduino
