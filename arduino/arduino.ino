#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN 9
#define SS_PIN 10

int servoPin = 3;
int buzzer = 2;
int buttonSignalPin = 8;

int r_led= 5;
int g_led = 6;
int b_led = 7;

int state = 0;

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo;

const char adminUID[] = "69581618"; 
const char userUID[] = "722BE051"; 

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  servo.attach(servoPin);

  pinMode(buttonSignalPin, INPUT);
  pinMode(buzzer, OUTPUT); 
  pinMode(r_led, OUTPUT);
  pinMode(g_led, OUTPUT);
  pinMode(b_led, OUTPUT);

  Serial.println("Scan RFID!");
}

void loop() {
  if(state == 0)
    ledColor(255, 0, 0);
  else
    ledColor(0, 255, 0);

  if (digitalRead(buttonSignalPin) == HIGH) {
    state = 1;
    soundBuzzer();
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  Serial.println("UID detectat: " + uid);

  if (uid == adminUID) {
    state = 0;
    rotateServo();
  }
  else if(uid == userUID && state == 1) {
    state = 0;
    rotateServo();
  }

  rfid.PICC_HaltA();
}

void ledColor(int rValue, int gValue, int bValue) {
  analogWrite(r_led, rValue);
  analogWrite(g_led, gValue);
  analogWrite(b_led, bValue);
}

void soundBuzzer() {
  tone(buzzer, 1000);
  delay(1000);
  noTone(buzzer);     
  delay(1000); 
}

void rotateServo() {
  tone(buzzer, 1000);
  servo.write(180);
  delay(1000);
  servo.write(0); 
  noTone(buzzer);     
  delay(1000); 
}
