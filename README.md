# Physical Computing Project 2025 - IT KMITL
  Project's Title : EasyMath - Games with microcontrollers
  - Project นี้เป็นส่วนหนึ่งในรายวิชา PHYSICAL COMPUTING 06016409 ภาคเรียนที่ 1 ปีการศึกษา 2568
    คณะเทคโนโลยีสารสนเทศ สาขาเทคโนโลยีสารสนเทศ สถาบันเทคโนโลยีพระจอมเกล้าเจ้าคุณทหารลาดกระบัง

## บทคัดย่อ
  เลขฐานสองเป็นพื้นฐานที่สำคัญในระบบคอมพิวเตอร์ แต่การเรียนในห้องเรียนกลับมีเนื้อหาที่ไม่ไม่น่าสนใจ โปรเจคนี้จึงถูกสร้างขึ้นเพื่อแก้ปัญหานี้ โดยเปลี่ยนการเรียนรู้ให้เป็นเกมที่สนุกและโต้ตอบได้ ผ่านการสุ่มโจทย์ผ่านทาง Website และกรอกข้อมูลได้ผ่านทาง Arduino ซึ่งได้นำเอาหลักการของเกมมาใช้ เพื่อทำให้เรื่องที่เข้าใจยากกลายเป็นเรื่องที่สนุกขึ้น

## วงจรใน Arduino
![image](https://github.com/KhanunKup/PC-PROJECT-Y2/blob/main/resources/tinkercad.png?raw=true)

![image](https://github.com/KhanunKup/PC-PROJECT-Y2/blob/main/resources/drive.png?raw=true)

## อุปกรณ์ที่ใช้

Arduino
![image](https://github.com/KhanunKup/PC-PROJECT-Y2/blob/8ecb4a5e3200306e91684e915195413c6bd5387d/resources/Arduino%20R4.png)

LCD
![image](https://github.com/KhanunKup/PC-PROJECT-Y2/blob/8ecb4a5e3200306e91684e915195413c6bd5387d/resources/LCD.png)

Switch
![image](https://github.com/KhanunKup/PC-PROJECT-Y2/blob/8ecb4a5e3200306e91684e915195413c6bd5387d/resources/Switch.png)

## Poster
![image](https://github.com/KhanunKup/PC-PROJECT-Y2/blob/main/resources/poster.png?raw=true)

## Clip VDO แนะนำโปรเจกต์

## Library ที่ใช้งาน
```c++
#include <WiFiS3.h>
#include <LiquidCrystal_I2C.h>
```

## ฟังก์ชั่นการเชื่อมต่อ Local WIFI
```c++
  // connect to local wifi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
```

## ผู้จัดทำ
|รหัสนักศึกษา|ชื่อ - นามสกุล|
|--|--|
| 67070001 | นาย กฤตภัทร	แจ่มวัฏกูล |
| 67070021 | นาย จิรพนธ์ วังยายฉิม |
| 67070041 | นาย ฐานพัชร์	ทิพย์เที่ยงแท้ |
| 67070044 | นาย ฐิติวุฒน์	ครุฑชื่น |
