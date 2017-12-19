#include <Wire.h>
#include "Waiter.cpp"
#include "Sensor.cpp"

#define DEBOUNCE_TIME 100
#define DELAY 3000 // задержка перед включением
#define ADDRESS 12

#define SOLVED 1
#define NOT_SOLVED 0

const int sensorsCount = 3;

Sensor sensors [sensorsCount] = {
  Sensor(4,DEBOUNCE_TIME),
  Sensor(5,DEBOUNCE_TIME),
  Sensor(6,DEBOUNCE_TIME)
};

Waiter* waiter = new Waiter(DELAY);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  
  for (int i = 0; i < sensorsCount; i++) {
    sensors[i].onChange(stateChanged);
  }
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  
  Serial.println("Started.");
}

void loop() {
  for (int i = 0; i < sensorsCount; i++) {
    sensors[i].check();
  }
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
    state = state && sensors[i].isActivated();
  }
  return state;
}

void stateChanged(int pin) {
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

