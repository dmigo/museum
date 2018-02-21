#define ADDRESS 24

#include <Wire.h>
#include "KeyCode.cpp"
#include "GameStateIndication.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

KeyCode* rightCode;
GameStateIndication* indication;

static const byte sizeRightPassword = 4; // длинна правильного пароля 
char rightPassword[sizeRightPassword] = {'3', '8', '2', '9'}; // правильный пароль

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  indication = new GameStateIndication(10, 11);// зеленый индикатор, красный индикатор
  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  rightCode->onSuccess(codeSolved);
  rightCode->onFailure(codeFailed);
  
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  rightCode->check();
  indication->check();
}

void requestEvent() {
  if(rightCode->isSolved())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

void codeSolved(){
 Serial.println("Win");
 indication->win();
}
void codeFailed(){
 Serial.println("Fail");
 indication->fail();
}

