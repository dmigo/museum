#include "Toolbox/Button.cpp"
#include "Toolbox/Indicators.cpp"

const int elementsCount = 4;
Button buttons [elementsCount] = {
  Button(4),
  Button(5),
  Button(6),
  Button(7),
};
SimpleIndicator indicators [elementsCount] = {
  SimpleIndicator(A1),
  SimpleIndicator(A2),
  SimpleIndicator(A3),
  SimpleIndicator(A4),
};
bool state [elementsCount] = {false};

int mapPinToId(int pin) { //ToDo replace with a map or so
  switch (pin) {
    case 7:
      return 0;
      break;
    case 8:
      return 1;
    case 9:
      return 2;
  }
}

void setup() {
  Serial1.begin(115200);
  Serial2.begin(9600);
  Serial1.println("Starting...");

  for (int i = 0; i < elementsCount; i++) {
    buttons[i].onRelease(handleButtonRelease);
  }

  Serial1.println("Started.");
}

void handleButtonRelease(int pin) {
  int id = mapPinToId(pin);
  Serial2.print("Button ");
  Serial2.print(pin);
  Serial2.println(" released");
  setState(id, true);
}

void setState(int id, bool newState) {
  state[id] = newState;
  sendState();
  render();
}

void sendState() {
  for (int i = 0; i < elementsCount; i++) {
    Serial1.print('C');
    Serial1.print(i);
    Serial1.print(':');
    Serial1.print(state[i]);
  }
}

void skipToCommand() {
  if (Serial1.available() <= 0)
    return;
    
  while(Serial1.peek() != 'C'){
    Serial1.read();
  }
}

void executeCommand() {
  if (Serial1.available() <= 0
  || Serial1.peek() != 'C')
    return;

  int id = Serial1.parseInt();
  Serial1.read();
  bool newState = Serial1.parseInt();

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
