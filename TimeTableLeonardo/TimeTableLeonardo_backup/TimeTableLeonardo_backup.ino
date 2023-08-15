#include <SoftwareSerial.h>
#include <Keyboard.h>

bool mic_status = false;
int step = 0;
SoftwareSerial bluetoothSerial(11, 10);  // RX, TX for HC-05 module
unsigned long previousMillis = 0;
const int intervals[] = { 500, 500, 500, 12000, 12000, 1000, 1000, 1000 };  // Time intervals for each step in milliseconds

void setup() {
  Serial.begin(9600);  // Initialize serial monitor
  bluetoothSerial.begin(9600);
  Keyboard.begin();
}

void loop() {
  if (bluetoothSerial.available()) {
    char receivedChar = bluetoothSerial.read();
    Serial.println(receivedChar);

    if (receivedChar == 'r') {
      Keyboard.write(KEY_RIGHT_ARROW);
    }
    if (receivedChar == 'l') {
      Keyboard.write(KEY_LEFT_ARROW);
    }
    if (receivedChar == 'x') {
      mic_status = true ;
      
    }
    Keyboard.write('1');
  }
  if (mic_status) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= intervals[step]) {
          previousMillis = currentMillis;
          step++;
          if (step == 1) {
            Serial.println("1");
            Keyboard.write('1');
          }
          if (step == 2) {
            Serial.println("2");
            Keyboard.write('2');
          }
          if (step == 3) {
            Serial.println("3");
            Keyboard.write('3');
          }
          if (step == 4) {
            Serial.println("4");
            Keyboard.write('4');
          }
          if (step == 5) {
            Serial.println("5");
            Keyboard.write('4');
          }
          if (step == 6) {
            Serial.println("6");
            Keyboard.write('3');
          }
          if (step == 7) {
            Serial.println("2");
            Keyboard.write('2');
          }
          if (step == 8) {
            Serial.println("1");
            Keyboard.write('1');
          }


          // ... other step cases ...

          if (step == 8) {
            step = 0;
            mic_status = false;
          }
        }
      }
}