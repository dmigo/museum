#define ADDRESS // 15 - 18
#define UID 10775353  //айдишник нужной нфцшки
#define LIGHT_UP_DURATION 1000

#include <Wire.h>
#include "RfidLock.cpp"
#include "SimpleIndicator.cpp"
#include "DurableIndicator.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

RfidLock* rfidLock;
DurableIndicator* green;

void setup()
{
  Serial.begin(9600);
  Serial.println("Version 1.0.2");
  Serial.println("Starting...");
  
  rfidLock = new RfidLock(UID);
  rfidLock->onActive(lightUp);

  SimpleIndicator* inner = new SimpleIndicator(A2); // пин индикатора
  green = new DurableIndicator(inner); 

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{ 
  rfidLock->check();
  green->check();
}

void requestEvent() {
  if(rfidLock->isOpen())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

void lightUp(unsigned long uid){
  Serial.print("Rfid: ");
  Serial.println(uid);
  green->lightUp(LIGHT_UP_DURATION);
}

