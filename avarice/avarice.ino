#define ADDRESS 14

#include <Wire.h>
#include "KeyCode.cpp"
#include "Arm.cpp"
#include "GameStateIndication.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

KeyCode* rightCode;
Arm* arm;
GameStateIndication* indication;

static const byte sizeRightPassword = 5; // длинна правильного пароля 
char rightPassword[sizeRightPassword] = {'3', '2', '1', '6', '7'}; // правильный пароль

const int sensorPin = 13; // пин сенсора
const int duplicatorPin = 4; // пин двойник

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  indication = new GameStateIndication(A1, A0);// зеленый индикатор, красный индикатор
  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  rightCode->onSuccess(codeSolved);
  rightCode->onFailure(codeFailed);
  arm = new Arm(9, 50, 93); // пин и диапазон для сервака
  
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), onSensor, CHANGE);
  pinMode(duplicatorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(duplicatorPin), onSensor, CHANGE);
  
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  rightCode->check();
  indication->check();
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

bool isSensorActivated(){
  return digitalRead(sensorPin) == LOW
  || digitalRead(duplicatorPin) == LOW;
}

void onSensor() {
  if (isSensorActivated())
    arm->toEnd();
  else
    arm->toStart();
}

void codeSolved(){
 Serial.println("Win");
 indication->win();
}
void codeFailed(){
 Serial.println("Fail");
 indication->fail();
}

