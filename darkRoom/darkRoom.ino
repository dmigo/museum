
#include <Wire.h>
#include "RfidLock.cpp"
#include "KeyCode.cpp"
#include "Indicators.cpp"

#define SOLVED 1
#define NOT_SOLVED 0
#define ADDRESS 9

RfidLock *rfidLock;
KeyCode *keyCode;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  rfidLock = new RfidLock(32157); //сюда кладем правильный ключ
  keyCode = new KeyCode();
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Starting...");
}

void loop()
{ 
  rfidLock->check();
  keyCode->check();
}

void requestEvent() {
  Serial.println("data requested");
  if(keyCode->isOpen() && rfidLock->isOpen())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

