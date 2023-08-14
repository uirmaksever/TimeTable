#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
uint8_t leonardoAddress[] = { 0x00, 0x22, 0x04, 0x00, 0xFF, 0x85 };
const int MICROPHONE_PIN = 33;
const int ROTARY1_PIN = 32;
const int ROTARY2_PIN = 35;
volatile int microphone_active_counter = 0;
volatile int encoderDirection = 0;

void encoderAISR();
void encoderBISR();
void microphoneISR();
char dataToSend = 'a';  // Change this variable to the desired value ('a' to 'k')

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT", true);
  pinMode(MICROPHONE_PIN, INPUT_PULLUP);
  pinMode(ROTARY1_PIN, INPUT_PULLUP);
  pinMode(ROTARY2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MICROPHONE_PIN), microphoneISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY1_PIN), encoderAISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY2_PIN), encoderBISR, CHANGE);
}

void loop() {
  SerialBT.connect(leonardoAddress);
  SerialBT.println(dataToSend);
  Serial.println("Data sent: " + String(dataToSend));
  Serial.println(microphone_active_counter);
  dataToSend = 'p';

  delay(100);  // Delay before sending the next data
}
void IRAM_ATTR encoderBISR() {
  int encoderAState = digitalRead(ROTARY1_PIN);
  int encoderBState = digitalRead(ROTARY2_PIN);

  if (encoderAState == encoderBState) {
    // Counterclockwise rotation
    encoderDirection = -1;
    Serial.println("COUNTER CLOCKWISE");
    dataToSend = 'r';

  } else {
    // Clockwise rotation
    encoderDirection = 1;
    Serial.println("CLOCKWISE");
    dataToSend = 'l';
  }
}
void IRAM_ATTR encoderAISR() {
  int encoderAState = digitalRead(ROTARY1_PIN);
  int encoderBState = digitalRead(ROTARY2_PIN);

  if (encoderAState == encoderBState) {
    // Clockwise rotation
    encoderDirection = 1;
    Serial.println("CLOCKWISE");
    dataToSend = 'l';

  } else {
    // Counterclockwise rotation
    encoderDirection = -1;
    Serial.println("COUNTER CLOCKWISE");
    dataToSend = 'r';
  }
}

void IRAM_ATTR microphoneISR() {
  int microphone_state = digitalRead(MICROPHONE_PIN);
  microphone_active_counter++;
  if (microphone_active_counter > 800) {
    // This is for filtering noise
    dataToSend = 'x';
    Serial.println("Microphone triggered.");
    microphone_active_counter = 0;
  }
}