#ifndef SENSOR
#define SENSOR

#include <Arduino.h>

class Sensor {
  private:
    int _state;
    long _stateTimestamp;
    long _debounceTime;
    int _pin;

    void (*_onDrop)(int);
    void (*_onRise)(int);
    void (*_onChange)(int);

    void _act() {
      if (_state == LOW) {
        _drop();
      } else {
          _rise();
      }
      _change();
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

    void _change() {
      if (_onChange != 0L) {
        _onChange(_pin);
      }
    }

    void _updateState() {
      _state = digitalRead(_pin);
      _stateTimestamp = millis();
    }

    bool _stateChanged() {
      print(" stateChanged - 1");
      return digitalRead(_pin) != _state;
    }
    bool _debounced() {
      print(" _debounced - 1");
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
      print(" check - 1");
      if (_stateChanged()
          && _debounced()) {
        print(" check - 2");
        _updateState();
        print(" check - 3");
        _act();
      }
      print(" check - 4");
    }
    void print(String msg){
      Serial.print("[");
      Serial.print(_pin);
      Serial.print("] ");
      Serial.println(msg);
    }

    void onDrop(void (*callback)(int)) {
      _onDrop = callback;
    }
    void onRise(void (*callback)(int)) {
      _onRise = callback;
    }
    void onChange(void (*callback)(int)) {
      _onChange = callback;
    }

    bool isActivated(){
      return _state == LOW;
    }
};
#endif
