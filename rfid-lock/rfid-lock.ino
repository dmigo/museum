#define ADDRESS 8
#define UID 10775353  //айдишник нужной нфцшки

#include <Wire.h>
#include "RfidLock.cpp"
#include "SimpleIndicator.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

RfidLock* rfidLock;
SimpleIndicator* green;

void setup()
{
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");
  
  rfidLock = new RfidLock(UID);
  
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

