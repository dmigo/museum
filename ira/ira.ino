#define ADDRESS 8

#include <Wire.h>
#include "RfidLock.cpp"
#include "Indicators.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

RfidLock* rfidLock;
SimpleIndicator* green;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  rfidLock = new RfidLock(32157); //сюда кладем правильный rfid-ключ
  
  green = new SimpleIndicator(A2); // пин индикатора

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{ 
  rfidLock->check();

  if(rfidLock->isOpen())
    green->switchOn();
  else
    green->switchOff();
}

void requestEvent() {
  if(rfidLock->isOpen())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

