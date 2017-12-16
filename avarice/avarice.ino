#define ADDRESS 14

#include <Wire.h>
#include "KeyCode.cpp"
#include "Arm.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

KeyCode* rightCode;

static const byte sizeRightPassword = 5; // длинна правильного пароля 
char rightPassword[sizeRightPassword] = {'3', '2', '1', '6', '7'}; // правильный пароль

Arm* arm;
const int sensorPin = 13;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  arm = new Arm(5, 50, 93);
  
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(0, onSensor, CHANGE);
  
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  rightCode->check();
}

bool isSolved() {
  return rightCode->isSolved();
}

void requestEvent() {
  if(isSolved())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

void onSensor() {
  if (digitalRead(sensorPin) == LOW)
    arm->toEnd();
  else
    arm->toStart();
}

