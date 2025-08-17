# People Counter with IR Limit (Arduino Uno + IR x2 + Relay Active-LOW + Buzzer + I2C LCD 16×2)

ระบบนับจำนวนคนเข้า–ออกด้วย IR 2 จุด (ลำดับ IR1→IR2 = เข้า, IR2→IR1 = ออก) แสดงผลบน LCD 16×2 (I2C)  
และควบคุมไฟ 12V ผ่านรีเลย์ เมื่อจำนวนเกินกำหนดจะเปลี่ยนสถานะไฟและส่งเสียงแจ้งเตือน

- เมื่อ ≤ 40 คน: ไฟ **เขียว** ติด  
- เมื่อ > 40 คน: ไฟ **แดง** ติด + Buzzer เตือนเป็นจังหวะ  

รีเลย์ที่ใช้เป็น Active-LOW (สั่งติดด้วย `LOW`, ดับด้วย `HIGH`)

---

## 📦 อุปกรณ์ที่ใช้
- Arduino Uno (R3 หรือเทียบเท่า)
- IR Sensor (เอาต์พุตดิจิทัล DO) × 2
- Relay Module 5V × 2 ช่อง (Active-LOW)
- Buzzer 5V (แบบ Active)
- หลอดไฟ 12V สีเขียว ×1, สีแดง ×1
- LCD 16×2 (I2C, ที่อยู่ทั่วไป 0x27 หรือ 0x3F)
- สายไฟ/อะแดปเตอร์ + แหล่งจ่าย 12V สำหรับหลอดไฟ

---

## 🔌 การเชื่อมต่อ

| อุปกรณ์ | ขา Arduino Uno |
|----------|----------------|
| IR1 (ด้านนับ) DO | D2 |
| IR2 (ด้านออก) DO | D3 |
| Relay CH1 (ไฟเขียว) IN1 | D6 |
| Relay CH2 (ไฟแดง) IN2 | D7 |
| Buzzer 5V (Active) | D9 |
| LCD 16×2 (I2C) SDA | A4 |
| LCD 16×2 (I2C) SCL | A5 |
| LCD VCC | 5V |
| LCD GND | GND |

> **หมายเหตุ**:  
> - ใช้ GND ร่วมระหว่าง Arduino, IR Sensor, Relay  
> - หลอดไฟ 12V ต่อผ่านรีเลย์ (COM/NO) โดยมีแหล่งจ่าย 12V แยก ไม่ใช้จากบอร์ด Arduino โดยตรง

---

## 🚀 Quick Start

1. ติดตั้งไลบรารี `LiquidCrystal_I2C` ผ่าน Library Manager  
2. ต่อสายตามตาราง (GND ร่วมระหว่าง Arduino–IR–Relay)  
3. อัปโหลด `PeopleCounter.ino` ไปยัง Arduino Uno  
4. เปิด **Serial Monitor (115200)** เพื่อตรวจสถานะ  
5. ทดสอบเดินเข้า (IR1→IR2) และออก (IR2→IR1)  

---

## ⚙️ Config

- เปลี่ยนเกณฑ์ความจุ: `CAP_LIMIT`  
- สลับรูปแบบรีเลย์: `RELAY_ACTIVE_LOW` (ตั้ง `false` หากรีเลย์ Active-HIGH)  
- กันเด้ง/หมดเวลา: `DEBOUNCE_MS`, `STEP_TIMEOUT_MS`  
- จังหวะเสียง: `BUZZ_ON_MS`, `BUZZ_OFF_MS`  
- หากโมดูล IR ให้ตรรกะกลับ (ถูกบัง = HIGH) ให้แก้ใน `isIR1Blocked()` / `isIR2Blocked()`  

---

## 🛠 Troubleshooting

- **LCD ไม่ขึ้น** → เช็ก I2C address (0x27/0x3F), สาย SDA/SCL (A4/A5), 5V/GND  
- **รีเลย์กลับด้าน** → ยืนยัน `RELAY_ACTIVE_LOW = true` สำหรับโมดูล Active-LOW  
- **นับผิด/เด้ง** → เพิ่ม `DEBOUNCE_MS`, ปรับระยะ IR1↔IR2 (30–60 ซม.), ใช้ปลอกบังแสง  
- **ไฟ 12V ไม่ติด** → เช็ก COM/NO บนรีเลย์ และแหล่งจ่าย 12V  

---

## 📸 ตัวอย่างการทำงาน
- จำนวนคน ≤ 40 → ไฟเขียวติด  
- จำนวนคน > 40 → ไฟแดง + Buzzer  

---
