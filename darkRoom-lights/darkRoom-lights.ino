#define ADDRESS 10

#include <Wire.h>
#include "Sensor.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

Sensor* sensor1;
Sensor* sensor2;
Sensor* sensor3;
Sensor* sensor4;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  sensor1 = new Sensor(8);
  sensor2 = new Sensor(9);
  sensor3 = new Sensor(10);
  sensor4 = new Sensor(11);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
}

void requestEvent() {
  if(sensor1->isSolved() 
  && sensor2->isSolved() 
  && sensor3->isSolved() 
  && sensor4->isSolved())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

