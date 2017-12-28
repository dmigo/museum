#include <Wire.h>
#include "SimpleIndicator.cpp"
#include "ConditionalTimer.cpp"
#include "Sensor.cpp"
#include "Puzzle.cpp"

#define DEBOUNCE_TIME 100
#define TIMER_INTERVAL 1500
#define ADDRESS 19

#define SOLVED 1
#define NOT_SOLVED 0

Sensor* sensor;
SimpleIndicator* green;
ConditionalTimer* timer;
Puzzle* puzzle;

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.1.0");
  Serial.println("Starting...");
  
  sensor = new Sensor(6, DEBOUNCE_TIME);
  green = new SimpleIndicator(7); // пин индикации
  green->switchOff();
  timer = new ConditionalTimer(isSensorActive, TIMER_INTERVAL);
  timer->onReached(timerReached);
  puzzle = new Puzzle();
  
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  
  Serial.println("Started.");
}

void loop() {
  sensor->check();
  timer->check();
}

void requestEvent() {
  if (puzzle->isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

bool isSensorActive() {
  Serial.print("Sensor is active: ");
  bool isActive = sensor->isActive();
  Serial.println(isActive);
  return isActive;
}

void timerReached() {
  Serial.println("Timer reached.");
  puzzle->solve();
  green->switchOn();
}

