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

[Watch the video](https://www.youtube.com/watch?v=1SWllteXLZ4)



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

## ฟังก์ชั่นที่สำคัญ
- Listener ของ Switch
```c++
void handleButtons() {
  if (digitalRead(btn1) == LOW) {   // press 1
    binaryString += "1"; 
    updateDisplay(); 
    delay(250);
  }
  if (digitalRead(btn0) == LOW) {  // press 0
    binaryString += "0"; 
    updateDisplay();
    delay(250); 
  }
  if (digitalRead(btnClear) == LOW) {  // press clear
    binaryString = ""; 
    updateDisplay(); 
    delay(250);
  }

  if (digitalRead(btnEnter) == LOW) {  // press enter ans
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ans:");
    lcd.setCursor(5, 0);
    lcd.print(correctAnswer);  // show correct ans
    lcd.setCursor(0, 1);
    
    if (binaryString == correctAnswer) {
      lcd.print("Correct!");
      lastResult = "correct";
    } else {
      lcd.print("Wrong!");
      lastResult = "wrong";
    }

    delay(2000);
    binaryString = "";
    updateDisplay();
  }
}
```

- Listener ระหว่าง Arduino & Browser
```c++
void handleWebServer() {
  WiFiClient client = server.available();  // check if it has new client
  if (client) {
    String currentLine = "";
    String requestPath = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // send response back
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println("Connection: close");
            client.println();

            if (requestPath.startsWith("/set?ans=")) {
              client.println("Answer Updated");
            } else if (requestPath.startsWith("/status")) {
              client.println(lastResult);
              // reset state of result
              if (lastResult != "none") {
                lastResult = "none";
              }
            } else {
              client.println("Hello from Arduino!");
            }
            break;
          } else {
            // read request
            if (currentLine.startsWith("GET ")) {
              int firstSpace = currentLine.indexOf(' ');
              int secondSpace = currentLine.indexOf(' ', firstSpace + 1);
              if (secondSpace > firstSpace) {
                requestPath = currentLine.substring(firstSpace + 1, secondSpace);
              }
              
              if (requestPath.startsWith("/set?ans=")) {
                int startIndex = requestPath.indexOf('=') + 1;
                correctAnswer = requestPath.substring(startIndex);
                lastResult = "none"; // reset state when random new quiz
                Serial.print("Updated correct answer to: ");
                Serial.println(correctAnswer);
              }
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}
```

## ผู้จัดทำ
|รหัสนักศึกษา|ชื่อ - นามสกุล|
|--|--|
| 67070001 | นาย กฤตภัทร	แจ่มวัฏกูล |
| 67070021 | นาย จิรพนธ์ วังยายฉิม |
| 67070041 | นาย ฐานพัชร์	ทิพย์เที่ยงแท้ |
| 67070044 | นาย ฐิติวุฒน์	ครุฑชื่น |
