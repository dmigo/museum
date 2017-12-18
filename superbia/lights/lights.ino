#include <Wire.h>
#include "Waiter.cpp"

#define DELAY 3000 // задержка перед включением
#define ADDRESS 12

#define SOLVED 1
#define NOT_SOLVED 0

const int sensorsCount = 3;
const int sensorPins[3] = {4,5,6};

Waiter* waiter = new Waiter(DELAY);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  
  for (int i = 0; i < sensorsCount; i++) {
    pinMode(sensorPins[i], INPUT);
    attachInterrupt(digitalPinToInterrupt(sensorPins[i]), stateChanged, CHANGE);
  }
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  
  Serial.println("Started.");
}

void loop() {

}

void requestEvent() {
  if (waiter->isFinished())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

bool isOpen() {
  bool state = true;
  for (int i = 0; i < sensorsCount; i++) {
    state = state && digitalRead(sensorPins[i]) == HIGH;
  }
  return state;
}

void stateChanged() {
  Serial.println("State changed.");
  if(isOpen()) {
    Serial.println("We are open.");
    waiter->start();
  }
  else {
    Serial.println("We are closed.");
    waiter->drop();
  }
}

