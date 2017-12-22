#include <Wire.h>
#include "SimpleIndicator.cpp"
#include "Sensor.cpp"

#define DEBOUNCE_TIME 100
#define ADDRESS 20

#define SOLVED 1
#define NOT_SOLVED 0

const int sensorsCount = 3;

Sensor sensors [sensorsCount] = {
  Sensor(4,DEBOUNCE_TIME),
  Sensor(5,DEBOUNCE_TIME),
  Sensor(6,DEBOUNCE_TIME)
};
SimpleIndicator* green = new SimpleIndicator(7); // пин индикации

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
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
  if (isOpen())
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
    green->switchOn();
  }
  else {
    Serial.println("We are closed.");
    green->switchOff();
  }
}

