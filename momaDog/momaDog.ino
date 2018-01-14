

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
          digitalWrite(pin, HIGH);
        else
          digitalWrite(pin, LOW);
      }

    }
    void init(Locks* locks) {
      for (int i = 0; i < locks->amount; i++) {
        pinMode(locks->items[i].pin, OUTPUT);
        digitalWrite(locks->items[i].pin, LOW);
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
        Serial.print(address);//todo remove
        Serial.println(" ??? ");//todo remove
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
        Serial.print(address);//todo remove
        Serial.print(" available ");//todo remove
        Serial.println(state);//todo remove
        return Action(ACTION_SET_PUZZLE_STATE, puzzle, state);
      }
      else {
        return Action(ACTION_SET_PUZZLE_STATE, puzzle, NOT_HERE);
      }
    }
};

//Main component ===========================


////Initial state ============================
Puzzle acedia_puzzles [1] = {
  Puzzle(7)
};
Lock acedia_lock(acedia_puzzles, 1, 7);

Puzzle gula_puzzles [2] = {
  Puzzle(8),
  Puzzle(9)
};
Lock gula_lock(gula_puzzles, 2, 8);

const int locksAmount = 2;
Lock locksArray [locksAmount] = {
  acedia_lock,
  gula_lock
};
Locks* locks = new Locks(locksAmount, locksArray);
//Initial state ============================

State* state = new State(locks);
Store* store = new Store(reduce, state);

void transmitMsg (String msg) {
  // todo uncomment Serial.println(msg);
}
Broadcaster* broadcaster = new Broadcaster(transmitMsg);
Renderer* renderer = new Renderer();
SerialCommander* serialCommander = new SerialCommander(&Serial); // Serial в зависимости от модели Arduino
I2CCommander* i2cCommander = new I2CCommander();

void setup () {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Version 1.0.0");
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
