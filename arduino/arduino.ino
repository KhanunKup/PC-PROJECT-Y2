#include <WiFiS3.h>
#include <LiquidCrystal_I2C.h>

// --- wifi config ---
char ssid[] = "IKhanun";       // wifi ssid
char pass[] = "IK2548rosi";   // wifi pass
int status = WL_IDLE_STATUS;
WiFiServer server(80); // initialize the server on port 80

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int btn1 = 0;
const int btn0 = 1;
const int btnClear = 2;
const int btnEnter = 3;

String binaryString = "";
String correctAnswer = "0000"; // value will be updated from the web

void setup() {
  Serial.begin(9600);
  
  pinMode(btn1, INPUT_PULLUP); // use internal pull-up resistors
  pinMode(btn0, INPUT_PULLUP);
  pinMode(btnClear, INPUT_PULLUP);
  pinMode(btnEnter, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  // --- connect to wifi ---
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  
  server.begin(); // start the web server
  Serial.println("Server started");
  printWifiStatus();

  lcd.setCursor(0, 0);
  lcd.print("Binary Input:");
  lcd.setCursor(0, 1);
  lcd.print(binaryString);
}

void loop() {
  handleWebServer(); // check for incoming web data
  handleButtons();   // check for button press
}

// --- handle web data ---
void handleWebServer() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          // if the line is blank, the HTTP request has ended
          if (currentLine.length() == 0) {
            // send a standard HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Access-Control-Allow-Origin: *"); // allow requests from any origin
            client.println();
            client.println("OK");
            break;
          } else {
            // check if the request contains our target path and parameter
            if (currentLine.startsWith("GET /set?ans=")) {
              // extract the binary string from the URL
              int startIndex = currentLine.indexOf('=') + 1;
              int endIndex = currentLine.indexOf(' ', startIndex);
              correctAnswer = currentLine.substring(startIndex, endIndex);
              
              Serial.print("Updated correct answer to: ");
              Serial.println(correctAnswer);
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop(); // close the connection
  }
}

// --- button function ---
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
    } else {
      lcd.print("Wrong!");
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