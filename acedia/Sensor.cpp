#include <Arduino.h>
#include "Blocker.cpp"

class Sensor {
  private:
    int _state;
    long _stateTimestamp;
    long _debounceTime;
    int _pin;

    void (*_onDrop)(int);
    void (*_onRise)(int);

    void _act() {
      if (_state == LOW) {
        _drop();
      } else {
        _rise();
      }
    }

    void _drop() {
      if (_onDrop != 0L) {
        _onDrop(_pin);
      }
    }

    void _rise() {
      if (_onRise != 0L) {
        _onRise(_pin);
      }
    }

    void _updateState() {
      _state = digitalRead(_pin);
      _stateTimestamp = millis();
    }

    bool _stateChanged() {
      return digitalRead(_pin) != _state;
    }
    bool _debounced() {
      return millis() > _stateTimestamp + _debounceTime;
    }

  public:
    Sensor(int pin, long debounceTime) {
      _pin = pin;
      _state = HIGH;
      _stateTimestamp = 0;
      _debounceTime = debounceTime;

      pinMode(pin, INPUT_PULLUP);
    }

    void check() {
      if (_stateChanged()
          && _debounced()) {

        _updateState();
        _act();
        
      }
    }

    void onDrop(void (*callback)(int)) {
      _onDrop = callback;
    }
    void onRise(void (*callback)(int)) {
      _onRise = callback;
    }
};

class BlockableSensor {
  private:
    Blocker* _blocker;
    Sensor* _sensor;
    
  public:
    BlockableSensor(int pin, long debounceTime) {
      _sensor = new Sensor(pin, debounceTime);
      _blocker = new Blocker();

      pinMode(pin, INPUT_PULLUP);
    }

    void check() {
      _blocker->check();
      _sensor->check();
    }

    void onDrop(void (*callback)(int)) {
      _sensor->onDrop(callback);
    }
    void onRise(void (*callback)(int)) {
      _sensor->onRise(callback);
    }

    void deactivate(long milliseconds) {
      _blocker->block(milliseconds);
    }
    void deactivate() {
      _blocker->block();
    }
    void activate() {
      _blocker->unblock();
    }
};

