#include <Arduino.h>
#include "Blocker.cpp"

class Sensor {
  private:
    int _state;
    long _stateTimestamp;
    long _debounceTime;
    int _pin;

    Blocker* _blocker;

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
      _blocker = new Blocker();

      pinMode(pin, INPUT_PULLUP);
    }

    void check() {
      if (_stateChanged()
          && _debounced()) {

        _updateState();

        if (_blocker->isBlocked())
          return;

        _act();
      }
    }

    void onDrop(void (*callback)(int)) {
      _onDrop = callback;
    }
    void onRise(void (*callback)(int)) {
      _onRise = callback;
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

