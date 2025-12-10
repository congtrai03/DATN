#include <Arduino.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <L298N.h>

// ---------- L298N PINS ----------
#define ENA 5
#define IN1 8
#define IN2 7

#define ENB 6
#define IN3 4
#define IN4 3

L298N motorA(ENA, IN1, IN2);
L298N motorB(ENB, IN3, IN4);

// ---------- NRF24 PINS ----------
RF24 radio(9, 10);  // CE, CSN

const byte address[6] = "00001";

// Struct nhận dữ liệu joystick
struct Data {
  int x;
  int y;
} data;

void setup() {
  Serial.begin(9600);

  // --- NRF24 ---
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // --- MOTOR ---
  motorA.stop();
  motorB.stop();

  Serial.println("Receiver ready");
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    int x = data.x - 512;  // cân giữa
    int y = data.y - 512;

    // map tốc độ 0 → 255
    int leftMotor  = constrain( map(y + x, -512, 512, -255, 255), -255, 255 );
    int rightMotor = constrain( map(y - x, -512, 512, -255, 255), -255, 255 );

    if (leftMotor > 0) {
      motorA.setSpeed(leftMotor);
      motorA.forward();
    } 
    else if (leftMotor < 0) {
      motorA.setSpeed(abs(leftMotor));
      motorA.backward();
    } 
    else {
      motorA.stop();
    }

    // --- MOTOR B ---
    if (rightMotor > 0) {
      motorB.setSpeed(rightMotor);
      motorB.forward();
    } 
    else if (rightMotor < 0) {
      motorB.setSpeed(abs(rightMotor));
      motorB.backward();
    } 
    else {
      motorB.stop();
    }
  }
}
