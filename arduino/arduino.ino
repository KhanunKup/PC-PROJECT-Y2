#include <WiFiS3.h>
#include <LiquidCrystal_I2C.h>

int status = WL_IDLE_STATUS;
WiFiServer server(80);

char ssid[] = "IKhanun";    //  ssid
char pass[] = "IK2548rosi"; //  password

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int btn1 = 0;
const int btn0 = 1;
const int btnClear = 2;
const int btnEnter = 3;

String binaryString = "";
String correctAnswer = "0000";
String lastResult = "none"; // keep last state

void setup() {
  Serial.begin(9600);
  
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn0, INPUT_PULLUP);
  pinMode(btnClear, INPUT_PULLUP);
  pinMode(btnEnter, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  // connect to local wifi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  
  server.begin();
  Serial.println("\nServer started");
  printWifiStatus();

  lcd.setCursor(0, 0);
  lcd.print("Binary Input:");
  lcd.setCursor(0, 1);
  lcd.print(binaryString);
}

void loop() { // seperate code to 2 segment
  handleWebServer();
  handleButtons();
}

void handleWebServer() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    String requestPath = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // -- send response back --
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
            // -- read request --
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

void handleButtons() {
  if (digitalRead(btn1) == LOW) {
    binaryString += "1"; 
    updateDisplay(); 
    delay(250); 
  }

  if (digitalRead(btn0) == LOW) {
    binaryString += "0"; 
    updateDisplay(); 
    delay(250); 
  }

  if (digitalRead(btnClear) == LOW) { 
    binaryString = ""; 
    updateDisplay(); 
    delay(250); 
  }

  if (digitalRead(btnEnter) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ans:");
    lcd.setCursor(5, 0);
    lcd.print(correctAnswer);
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

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Binary Input:");
  lcd.setCursor(0, 1);
  lcd.print(binaryString);
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}