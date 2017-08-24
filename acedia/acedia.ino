#define ADDRESS 11

#define SOLVED 1
#define NOT_SOLVED 0

#include <Wire.h>
#include "Button.cpp"
#include "Sequence.cpp"

const int buttonsCount = 6;
Button buttons [buttonsCount] = {
  Button(7),
  Button(8),
  Button(9),
  Button(10),
  Button(11),
  Button(12),
};

const int sequenceSize = 6;
int pinsSequence[sequenceSize] = {12, 11, 10, 9, 8, 7};

Sequence sequence(buttonsCount, pinsSequence);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Starting...");
  
  for(int i=0; i<buttonsCount; i++){
    buttons[i].onRelease(handleButtonClick);
  }
  sequence.onSuccess(handleSuccess);
  sequence.onFailure(handleFail);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  
  Serial.println("Started.");
}

void handleButtonClick(int pin){
  Serial.print("Button ");
  Serial.print(pin);
  Serial.println(" clicked");
  sequence.add(pin);
}

void handleSuccess(){
  Serial.println("*Success*");
}

void handleFail(){
  Serial.println("*Fail*");
}

void loop() {
  for(int i=0; i<buttonsCount; i++){
    buttons[i].check();
  }
}

void requestEvent() {
  if(sequence.isSolved())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}
