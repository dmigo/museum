#define ADDRESS 11

#define SOLVED 1
#define NOT_SOLVED 0

#define PLAYER_RX 0
#define PLAYER_TX 1

#include <Wire.h>

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#include "Button.cpp"
#include "Indicators.cpp"
#include "Sequence.cpp"

int pinToId(int pin) {
  switch (pin) {
    case 4:
      return 0;
      break;
    case 5:
      return 1;
      break;
    case 6:
      return 2;
      break;
    case 7:
      return 3;
      break;
    case 8:
      return 4;
      break;
    case 9:
      return 5;
      break;
  }
}

SoftwareSerial mySoftwareSerial(PLAYER_RX, PLAYER_TX); // сюда подключаем плэйер

const int fragmentsCount = 6;
Button buttons [fragmentsCount] = {
  Button(4),
  Button(5),
  Button(6),
  Button(7),
  Button(8),
  Button(9),
};
SimpleIndicator indicators [fragmentsCount] = {
  SimpleIndicator(A0),
  SimpleIndicator(A1),
  SimpleIndicator(A2),
  SimpleIndicator(A3),
  SimpleIndicator(A4),
  SimpleIndicator(A5),
};
Button exampleButton(12);
SimpleIndicator exampleIndicator(13);

BlinkingIndicator* red;
SimpleIndicator* green;

int fragments [fragmentsCount] = { // фрагменты трэка
  1,
  2,
  3,
  4,
  5,
  6
};
int completeSong = 7;
int durations [fragmentsCount] = { // длинны фрагментов
  1000,
  1000,
  1000,
  1000,
  1000,
  1000
};
int completeSongDuration = 6000;

const int sequenceSize = 6; // длинна правильной последовательности
int pinsSequence[sequenceSize] = {5, 4, 3, 2, 1, 0}; // правильная последовательность

Sequence sequence(fragmentsCount, pinsSequence);

void setup() {
  mySoftwareSerial.begin(9600);

  mp3_set_serial(mySoftwareSerial);
  mp3_set_volume(10);  // громкость крутить здесь

  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].onPress(handleButtonPress);
    buttons[i].onRelease(handleButtonRelease);
  }
  SimpleIndicator* simpleRed = new SimpleIndicator(10);
  red = new BlinkingIndicator(simpleRed, 1000, 500);
  green = new SimpleIndicator(11);
  
  exampleButton.onPress(handleExampleButtonPress);
  exampleButton.onRelease(handleExampleButtonRelease);
  sequence.onSuccess(handleSuccess);
  sequence.onFailure(handleFail);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
}

void deactivateAll(int duration) {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].deactivate(duration);
  }
  exampleButton.deactivate(duration);
}

void deactivateAll() {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].deactivate();
  }
  exampleButton.deactivate();
}

void handleButtonPress(int pin) {
  int id = pinToId(pin);
  indicators[id].switchOn();
}


void handleButtonRelease(int pin) {
  int id = pinToId(pin);
  sequence.add(id);
  mp3_play(fragments[id]);
  deactivateAll(durations[id]);
  indicators[id].switchOff();
}

void handleExampleButtonPress(int pin) {
  exampleIndicator.switchOn();
  mp3_play(completeSong);
  deactivateAll(completeSongDuration);
}


void handleExampleButtonRelease(int pin) {
  exampleIndicator.switchOff();
  mp3_play(completeSong);
  deactivateAll(completeSongDuration);
}

void handleSuccess() {
  mp3_play(completeSong);
  deactivateAll();
  green->switchOn();
}

void handleFail() {
  red->blinkNTimes(3);
}

void loop() {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].check();
  }
  exampleButton.check();
  red->check();
}

void requestEvent() {
  if (sequence.isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}
