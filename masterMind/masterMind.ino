#include "Toolbox/Button.cpp"
#include "Toolbox/Indicators.cpp"

const int elementsCount = 3;
Button buttons [elementsCount] = {
  Button(50),
  Button(51),
  Button(52),
};
SimpleIndicator indicators [elementsCount] = {
  SimpleIndicator(20),
  SimpleIndicator(21),
  SimpleIndicator(22),
};
bool state [elementsCount] = {false};

int mapPinToId(int pin) { //ToDo replace with a map or so
  switch (pin) {
    case 50:
      return 0;
    case 51:
      return 1;
    case 52:
      return 2;
  }
}

void setup() {
  Serial1.begin(9600);
  Serial2.begin(115200);
  Serial1.println("Starting...");

  for (int i = 0; i < elementsCount; i++) {
    buttons[i].onRelease(handleButtonRelease);
  }

  Serial1.println("Started.");
}

void handleButtonRelease(int pin) {
  int id = mapPinToId(pin);
  Serial1.print("Button ");
  Serial1.print(pin);
  Serial1.println(" released");
  setState(id, true);
}

void setState(int id, bool newState) {
  state[id] = newState;
  sendState();
  render();
}

void sendState() {
  for (int i = 0; i < elementsCount; i++) {
    Serial2.print('C');
    Serial2.print(i);
    Serial2.print(':');
    Serial2.print(state[i]);
  }
}

void skipToCommand() {
  if (Serial2.available() <= 0)
    return;
    
  while(Serial2.peek() != 'C'){
    Serial2.read();
  }
}

void executeCommand() {
  if (Serial2.available() <= 0
  || Serial2.peek() != 'C')
    return;

  int id = Serial2.parseInt();
  Serial2.read();
  bool newState = Serial2.parseInt();

  state[id] = state[id] || newState;
}

void readState() {
  skipToCommand();
  executeCommand();
  sendState();
  render();
}

void render() {
  for (int i = 0; i < elementsCount; i++) {
    if(state[i])
      indicators[i].switchOn();
    else
      indicators[i].switchOff();
  }
}

void loop() {
  readState();
  delay(10);
}
