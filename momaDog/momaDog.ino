

//Models for State and Actions    ============================

#define SOLVED 1
#define NOT_SOLVED 0
#define NOT_HERE -1

struct Puzzle {
  int address;
  int state = NOT_HERE;
  Puzzle(int aaddress) {
    address = aaddress;
  }
  bool isSolved() {
    return state == SOLVED;
  }
};


#define OPEN 1
#define CLOSED 0

struct Lock {
  private:
    bool _forced = false;
    bool _isOpen() {
      if (_forced) {
        return true;
      }
      else {
        bool result = true;
        for (int i = 0; i < puzzlesAmount; i++) {
          result = result && puzzles[i].isSolved();
        }
        return result;
      }
    }
  public:
    Puzzle* puzzles;
    int puzzlesAmount;
    int pin;

    Lock(Puzzle* apuzzles, int apuzzlesAmount, int apin) {
      puzzles = apuzzles;
      puzzlesAmount = apuzzlesAmount;
      pin = apin;
    }

    int getState() {
      if (_isOpen())
        return OPEN;
      else
        return CLOSED;
    }
    void setState(int state) {
      if (state == OPEN)
        _forced = true;
    }
};

struct Locks {
  Lock* items;
  int amount;
  Locks(int amountOfLocks, Lock* locks) {
    amount = amountOfLocks;
    items = locks;
  }
};

struct State {
  Locks* locks;

  State(Locks* alocks) {
    locks = alocks;
  }
};

#define ACTION_SET_PUZZLE_STATE 1
#define ACTION_SET_LOCK_STATE 2

struct Action {
  int type;

  int id;
  int state;

  Action(int atype, int aid, int astate) {
    type = atype;

    id = aid;
    state = astate;
  }
};

//Store        ============================

class Store {
  private:
    State* _state;
    State* (*_reducer)(State*, Action);
    void (*_onChange)();
  public:
    Store(State * (*reducer)(State*, Action) , State* initialState) {
      _state = initialState;
      _reducer = reducer;
      _onChange = 0L;
    }

    void dispatch(Action& action) {
      _state = _reducer(_state, action);

      if (_onChange != 0L)
        _onChange();
    }

    void onChange(void (*onChange)()) {
      _onChange = onChange;
    }

    State* getState() {
      return _state;
    }
};

//Reducer             ======================

State* reduce (State* state, Action action) {
  switch (action.type) {
    case ACTION_SET_LOCK_STATE:
      if (action.state == OPEN) {
        for (int i = 0; i < state->locks->amount; i++) {
          if (state->locks->items[i].pin == action.id)
            state->locks->items[i].setState(action.state);
        }
      }
    case ACTION_SET_PUZZLE_STATE:
      for (int i = 0; i < state->locks->amount; i++) {
        Lock lock = state->locks->items[i];
        for (int j = 0; j < lock.puzzlesAmount; j++) {
          if (lock.puzzles[j].address == action.id) {
            if (lock.puzzles[j].state != SOLVED) {
              lock.puzzles[j].state = action.state;
            }
          }

        }
      }
  }
  return state; //Would be awesome to clone the state here, though not sure if it is not too much atm
}


//IO             ============================

class Broadcaster {
  private:
    void (*_transmit)(String msg);
    String _getMsg(Lock lock) {
      String msg = "C";
      msg += lock.pin;
      msg += ":";
      msg += lock.getState();

      return msg;
    }

  public:
    Broadcaster(void (*transmit)(String msg)) {
      _transmit = transmit;
    }
    void report(Locks* locks) {
      for ( int i = 0; i < locks->amount; i++) {
        String msg = _getMsg(locks->items[i]);
        _transmit(msg);
      }
    }
};

class Renderer {
  private:
  public:
    Renderer() {

    }
    void render(Locks* locks) { //consider using controls with a render function ;)
      for (int i = 0; i < locks->amount; i++) {
        int pin = locks->items[i].pin;
        bool isOpen = locks->items[i].getState() == OPEN;
        if (isOpen)
          digitalWrite(pin, LOW);
        else
          digitalWrite(pin, HIGH);
      }

    }
    void init(Locks* locks) {
      for (int i = 0; i < locks->amount; i++) {
        pinMode(locks->items[i].pin, OUTPUT);
        digitalWrite(locks->items[i].pin, HIGH);
      }
    }
};

class SerialCommander {
  private:
    HardwareSerial* _serial;
  public:
    SerialCommander(HardwareSerial* serial) {
      _serial = serial;
    }
    bool available() {
      return _serial->available() > 0 && _serial->peek() == 'C';
    }
    Action getAction() {
      int lock = _serial->parseInt();
      _serial->read();
      int state = _serial->parseInt();

      return Action(ACTION_SET_LOCK_STATE, lock, state);// this class shouldn't know anything about locks and states
    }
};

#include <Wire.h>
class I2CCommander {
  private:
    bool _available(int address) {
      Wire.requestFrom(address, 1);
      return Wire.available();
    }
    byte _getState(int address) {
      return Wire.read();
    }
  public:
    I2CCommander() {
      Wire.begin();
    }
    Action getAction(int address) {
      int puzzle = address;
      if (_available(address)) { // this class shouldn't know anything about locks and states
        byte state = _getState(address);
        return Action(ACTION_SET_PUZZLE_STATE, puzzle, state);
      }
      else {
        return Action(ACTION_SET_PUZZLE_STATE, puzzle, NOT_HERE);
      }
    }
};

//Main component ===========================


////Initial state ============================
Puzzle ira_puzzles [1] = {
  Puzzle(8)
};
Lock ira_lock(ira_puzzles, 1, 8);

Puzzle darkRoom_puzzles [1] = {
  Puzzle(9)
};
Lock darkRoom_lock(darkRoom_puzzles, 1, 9);

Puzzle darkRoom_lights_puzzles [1] = {
  Puzzle(10)
};
Lock darkRoom_lights_lock(darkRoom_lights_puzzles, 1, 10);

Puzzle acedia_puzzles [1] = {
  Puzzle(11)
};
Lock acedia_lock(acedia_puzzles, 1, 11);

Puzzle superbia_lights_puzzles [1] = {
  Puzzle(12)
};
Lock superbia_lights_lock(superbia_lights_puzzles, 1, 12);

Puzzle superbia_code_puzzles [1] = {
  Puzzle(13)
};
Lock superbia_code_lock(superbia_code_puzzles, 1, 13);

Puzzle avarice_puzzles [1] = {
  Puzzle(14)
};
Lock avarice_lock(avarice_puzzles, 1, 14);

Puzzle chest_1_puzzles [1] = {
  Puzzle(15)
};
Lock chest_1_lock(chest_1_puzzles, 1, 15);

Puzzle chest_2_puzzles [1] = {
  Puzzle(16)
};
Lock chest_2_lock(chest_2_puzzles, 1, 16);

Puzzle chest_3_puzzles [1] = {
  Puzzle(17)
};
Lock chest_3_lock(chest_3_puzzles, 1, 17);

Puzzle chest_4_puzzles [1] = {
  Puzzle(18)
};
Lock chest_4_lock(chest_4_puzzles, 1, 18);

Puzzle mirror_puzzles [1] = {
  Puzzle(19)
};
Lock mirror_lock(mirror_puzzles, 1, 19);

Puzzle crucifix_puzzles [1] = {
  Puzzle(20)
};
Lock crucifix_lock(crucifix_puzzles, 1, 20);

Puzzle gula_candy_puzzles [1] = {
  Puzzle(21)
};
Lock gula_candy_lock(gula_candy_puzzles, 1, 21);

Puzzle gula_fruit_puzzles [2] = {
  Puzzle(22),
  Puzzle(23)
};
Lock gula_fruit_lock(gula_fruit_puzzles, 2, 22);

const int locksAmount = 15;
Lock locksArray [locksAmount] = {
  ira_lock,
  darkRoom_lock,
  darkRoom_lights_lock,
  acedia_lock,
  superbia_lights_lock,
  superbia_code_lock,
  avarice_lock,
  chest_1_lock,
  chest_2_lock,
  chest_3_lock,
  chest_4_lock,
  mirror_lock,
  crucifix_lock,
  gula_candy_lock,
  gula_fruit_lock
};
Locks* locks = new Locks(locksAmount, locksArray);
//Initial state ============================

State* state = new State(locks);
Store* store = new Store(reduce, state);

void transmitMsg (String msg) {
  Serial1.println(msg);
}
Broadcaster* broadcaster = new Broadcaster(transmitMsg);
Renderer* renderer = new Renderer();
SerialCommander* serialCommander = new SerialCommander(&Serial1); // Serial в зависимости от модели Arduino
I2CCommander* i2cCommander = new I2CCommander();

void setup () {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Version 1.0.1");
  renderer->init(store->getState()->locks);
  Serial.println("Started");
}


void loop() {
  State* currentState = store->getState();

  renderer->render(currentState->locks);
  broadcaster->report(currentState->locks);

  while (serialCommander->available()) {
    Action action = serialCommander->getAction();
    store->dispatch(action);
  }

  for (int i = 0; i < currentState->locks->amount; i++) { // deserves a separate class or function
    Lock lock = currentState->locks->items[i];
    for (int j = 0; j < lock.puzzlesAmount; j++) {
      int address = lock.puzzles[j].address;
      Action action = i2cCommander->getAction(address);
      store->dispatch(action);
    }
  }
}
