#include <SoftwareSerial.h>
#include <Keyboard.h>

bool mic_status = false;
int step = 0;
SoftwareSerial bluetoothSerial(11, 10);  // RX, TX for HC-05 module
unsigned long previousMillis = 0;
unsigned long previousMillisForRotation = 0;

char lastRotationChar = '\0';

// For each second until 3 seconds, 1, 2, 3 keys pressed consecutilvely.
// 4 is pressed twice after each 5 seconds
const int intervals[] = { 1000, 1000, 1000, 5000, 5000};  // Time intervals for each step in milliseconds

void setup() {
  Serial.begin(9600);  // Initialize serial monitor
  bluetoothSerial.begin(9600);
  Keyboard.begin();
  Serial.println("Receiver started.");
}

void loop() {
  if (bluetoothSerial.available()) {
    char receivedChar = bluetoothSerial.read();
    // Serial.println(receivedChar);
    Serial.println("Received char: " + String(receivedChar) + " -- Last char change: " + String(lastRotationChar));

    unsigned long currentMillisForRotation = millis();

    if (receivedChar == 'r') {
      // Keyboard.write(KEY_RIGHT_ARROW);
      Keyboard.press(KEY_RIGHT_ARROW);
      lastRotationChar = 'r';
      Serial.println("Pressed right.");
    }
    if (receivedChar == 'l') {
      // Keyboard.write(KEY_LEFT_ARROW);
      Keyboard.press(KEY_LEFT_ARROW);
      lastRotationChar = 'l';
      Serial.println("Pressed left.");
    }
    // The interval between two char sends from ESP should be tracked here. 
    //If the same chars are received between 2 or 3 sends, pressed key should be released.
    // Ideally, it should be equal to the duration of one loop for ESP, but it should be measured first.
    // But one can uses the total value of delays in the loop function of ESP. So 100 ms * 2.
    if (lastRotationChar != receivedChar && currentMillisForRotation - previousMillisForRotation > 200) {
      Keyboard.releaseAll();
      previousMillisForRotation = currentMillisForRotation;
      Serial.println("Released left or right key.");
    }

    if (currentMillisForRotation - previousMillisForRotation > 1000) {
      Keyboard.releaseAll();
    }

    if (receivedChar == 'x') {
      mic_status = true ;
      
    }
    Keyboard.write('1');
  } else {
    Serial.println("No device connected yet.");
  }
  // If mic is active
  if (mic_status) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= intervals[step]) {
          // Reset current time
          previousMillis = currentMillis;
          step++;
          if (step == 1) {
            Serial.println("Clicked 1.");
            Keyboard.write('1');
          }
          if (step == 2) {
            Serial.println("Clicked 2.");
            Keyboard.write('2');
          }
          if (step == 3) {
            Serial.println("Clicked 3");
            Keyboard.write('3');
          }
          if (step == 4) {
            Serial.println("Clicked 4.");
            Keyboard.write('4');
          }
          if (step == 5) {
            Serial.println("5");
            Keyboard.write('4');
          }
          // if (step == 6) {
          //   Serial.println("6");
          //   Keyboard.write('3');
          // }
          // if (step == 7) {
          //   Serial.println("2");
          //   Keyboard.write('2');
          // }
          // if (step == 8) {
          //   Serial.println("1");
          //   Keyboard.write('1');
          // }


          // ... other step cases ...

          if (step == 5) { // 8
            step = 0;
            mic_status = false;
          }
        }
      }
}