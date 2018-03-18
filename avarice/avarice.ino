#define ADDRESS 14
#define DROP_BUTTON A3
#define DEBOUNCE_TIME 100

#include <Wire.h>
#include "KeyCode.cpp"
#include "Arm.cpp"
#include "GameStateIndication.cpp"
#include "Sensor.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

KeyCode* rightCode;
Arm* arm;
GameStateIndication* indication;

static const byte sizeRightPassword = 4; // длинна правильного пароля
char rightPassword[sizeRightPassword] = {'1', '9', '8', '6'}; // правильный пароль

Sensor* sensor = new Sensor(13, DEBOUNCE_TIME); // сенсор
Sensor* duplicator = new Sensor(4, DEBOUNCE_TIME); // двойник
Sensor* dropButton = new Sensor(DROP_BUTTON, DEBOUNCE_TIME);  // кнопка сброса


void setup()
{
  Serial.begin(9600);
  Serial.println("Version 0.1.3");
  Serial.println("Starting...");

  indication = new GameStateIndication(A1, A0);// зеленый индикатор, красный индикатор
  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  rightCode->onSuccess(codeSolved);
  rightCode->onFailure(codeFailed);
  arm = new Arm(6, 130, 70); // пин и диапазон для сервака

  sensor->onDrop(onSensorDropped);
  duplicator->onDrop(onSensorDropped);
  sensor->onRise(onSensorRisen);
  duplicator->onRise(onSensorRisen);
  dropButton->onDrop(dropCode);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  dropButton->check();
  sensor->check();
  duplicator->check();
  rightCode->check();
  indication->check();
}

bool isSolved() {
  return rightCode->isSolved();
}

void requestEvent() {
  if (isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

void onSensorDropped(int pin) {
  Serial.print("Dropped: ");
  Serial.println(pin);
  arm->toEnd();
}

void onSensorRisen(int pin) {
  Serial.print("Risen: ");
  Serial.println(pin);
  arm->toStart();
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
  rightCode->restart();
}

