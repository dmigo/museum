#define ADDRESS 11

#define SOLVED 1
#define NOT_SOLVED 0

#define PLAYER_RX 10
#define PLAYER_TX 11

#include <Wire.h>

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include "Button.cpp"
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
DFRobotDFPlayerMini myDFPlayer;

const int fragmentsCount = 6;
Button buttons [fragmentsCount] = {
  Button(4),
  Button(5),
  Button(6),
  Button(7),
  Button(8),
  Button(9),
};

int fragments [fragmentsCount] = { // фрагменты трэка
  1,
  2,
  3,
  4,
  5,
  6
};
int durations [fragmentsCount] = { // длинны фрагментов
  1000,
  1000,
  1000,
  1000,
  1000,
  1000
};

const int sequenceSize = 6; // длинна правильной последовательности
int pinsSequence[sequenceSize] = {5, 4, 3, 2, 1, 0}; // правильная последовательность

Sequence sequence(fragmentsCount, pinsSequence);

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200); // ВНИМАНИЕ!!! серийник на вывод на необычной частоте

  Serial.println("Starting...");

  checkPlayerState();
  myDFPlayer.volume(10);  // громкость крутить здесь

  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].onPress(handleButtonPress);
    buttons[i].onRelease(handleButtonRelease);
  }
  sequence.onSuccess(handleSuccess);
  sequence.onFailure(handleFail);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);

  Serial.println("Started.");
}

void checkPlayerState() {
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));
}

void handleButtonPress(int pin) {
  int id = pinToId(pin);

  Serial.print("Button ");
  Serial.print(pin);
  Serial.println(" pressed");
}


void handleButtonRelease(int pin) {
  int id = pinToId(pin);

  Serial.print("Button ");
  Serial.print(pin);
  Serial.println(" released");

  sequence.add(id);
  myDFPlayer.play(fragments[id]);
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[id].deactivate(durations[id]);
  }
}

void handleSuccess() {
  Serial.println("*Success*");
}

void handleFail() {
  Serial.println("*Fail*");
}

void loop() {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].check();
  }
}

void requestEvent() {
  if (sequence.isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}
