#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN

const byte address[6] = "00001";

struct Data {
  int x;
  int y;
} data;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  data.x = analogRead(A0);
  data.y = analogRead(A1);

  radio.write(&data, sizeof(data));

  delay(10);
}
