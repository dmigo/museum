#define ADDRESS 25
#define DROP_BUTTON A3
#define DEBOUNCE_TIME 100

#include <Wire.h>
#include "KeyCode.cpp"
#include "GameStateIndication.cpp"
#include "Sensor.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

KeyCode* rightCode;
GameStateIndication* indication;
Sensor* dropButton;

static const byte sizeRightPassword = 4; // длинна правильного пароля
char rightPassword[sizeRightPassword] = {'2', '9', '7', '5'}; // правильный пароль

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");

  dropButton = new Sensor(DROP_BUTTON, DEBOUNCE_TIME); // кнопка сброса
  dropButton->onDrop(dropCode);

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
  dropButton->check();
  rightCode->check();
  indication->check();
}

void requestEvent() {
  if (rightCode->isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

void codeSolved() {
  Serial.println("Win");
  indication->win();
}
void codeFailed() {
  Serial.println("Fail");
  indication->fail();
}

void dropCode(int pin) {
  Serial.println("Drop");
  rightCode->drop();
}
