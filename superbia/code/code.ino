#define ADDRESS 13

#include <Wire.h>
#include "KeyCode.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

KeyCode* rightCode;

static const byte sizeRightPassword = 5; // длинна правильного пароля 
char rightPassword[sizeRightPassword] = {'р', 'а', 'й', 'с', 'к', 'и', 'й'}; // правильный пароль

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  
  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  rightCode->check();
}
void requestEvent() {
  if(rightCode->isSolved())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}
