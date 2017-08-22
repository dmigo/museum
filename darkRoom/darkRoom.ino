
#include <Wire.h>
#include "RfidLock.cpp"
#include "KeyCode.cpp"
#include "Indicators.cpp"

#define SOLVED 1
#define NOT_SOLVED 0
#define ADDRESS 9

RfidLock* rfidLock;
KeyCode* rightCode;

SimpleIndicator* green1;
SimpleIndicator* green2;
SimpleIndicator* red;
BlinkingIndicator* redBlink;

static const byte sizeRightPassword = 5; // длинна правильного пароля 
char rightPassword[sizeRightPassword] = {'3', '2', '1', '6', '7'}; // правильный пароль

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  rfidLock = new RfidLock(32157); //сюда кладем правильный rfid-ключ
  
  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  rightCode->onSuccess(codeSolved);
  rightCode->onFailure(codeFailed);
  
  green1 = new SimpleIndicator(A2);
  green2 = new SimpleIndicator(A1);
  red = new SimpleIndicator(A0);
  
  redBlink = new BlinkingIndicator(red, 500, 500);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{ 
  rfidLock->check();
  redBlink->check();
  rightCode->check();

  if(rfidLock->isOpen())
    green1->switchOn();
  else
    green1->switchOff();
}

void requestEvent() {
  if(rightCode->isSolved() && rfidLock->isOpen())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

void codeSolved(){
  green2->switchOn();
}
void codeFailed(){
  redBlink->blinkNTimes(3);
}
