#include "Toolbox/Button.cpp"
#include "Toolbox/Indicators.cpp"

const int elementsCount = 10;
Button buttons [elementsCount] = {
  Button(20),
  Button(21),
  Button(22),
  Button(23),
  Button(24),
  Button(25),
  Button(26),
  Button(27),
  Button(28),
  Button(29),
};
SimpleIndicator indicators [elementsCount] = {
  SimpleIndicator(40),
  SimpleIndicator(41),
  SimpleIndicator(42),
  SimpleIndicator(43),
  SimpleIndicator(44),
  SimpleIndicator(45),
  SimpleIndicator(46),
  SimpleIndicator(47),
  SimpleIndicator(48),
  SimpleIndicator(49),
};
bool state [elementsCount] = {false};

int mapPinToId(int pin) { //ToDo replace with a map or so
  switch (pin) {
    case 20:
      return 0;
    case 21:
      return 1;
    case 22:
      return 2;
  }
}

void setup() {
  Serial1.begin(9600);
  for (int i = 0; i < elementsCount; i++) {
    buttons[i].onPress(handleButtonPress);
    buttons[i].onRelease(handleButtonRelease);
  }
}

void handleButtonPress(int pin) {
  int id = mapPinToId(pin);
  setState(id, true);
}

void handleButtonRelease(int pin) {
}

void setState(int id, bool newState) {
  state[id] = newState;
  sendState();
  render();
}

void sendState() {
  Serial1.println();
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

  while (Serial1.peek() != 'C') {
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
    if (state[i])
      indicators[i].switchOn();
    else
      indicators[i].switchOff();
  }
}

void loop() {
  readState();
  for (int i = 0; i < elementsCount; i++) {
    buttons[i].check();
  }
}
