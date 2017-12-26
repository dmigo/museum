#define ADDRESS 21
#define SOLVED 1
#define NOT_SOLVED 0

#define DEBOUNCE 100

#include <Wire.h>
#include "Puzzle.cpp"
#include "Sensor.cpp"

Sensor* sensor = new Sensor(7, DEBOUNCE);
Puzzle* puzzle = new Puzzle();

void setup()
{
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");

  sensor->onChange(sensorChanged);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  sensor->check();
}

void requestEvent() {
  if (puzzle->isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

void sensorChanged(int pin) {
  if(sensor->isActive())
    puzzle->solve();
}
