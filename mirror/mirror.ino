#include <Wire.h>
#include "SimpleIndicator.cpp"
#include "Sensor.cpp"

#define DEBOUNCE_TIME 100
#define ADDRESS 12

#define SOLVED 1
#define NOT_SOLVED 0

Sensor* sensor = new Sensor(6,DEBOUNCE_TIME);
SimpleIndicator* green = new SimpleIndicator(7); // пин индикации

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");
  
  sensor->onChange(stateChanged);
  
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  
  Serial.println("Started.");
}

void loop() {
  sensor->check();
}

void requestEvent() {
  if (isOpen())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

bool isOpen() {
  return sensor->isActivated();
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

