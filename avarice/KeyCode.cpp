#ifndef KEYCODE
#define KEYCODE

#include <Keypad.h>

class Blocker{
  private:
  bool _blocked;
  long _whenBlocked;
  long _duration;

  public:
  Blocker(){
    _blocked = false;
  }
  void block(long milliseconds){
    _whenBlocked = millis();
    _duration = milliseconds;
    _blocked = true;
  }
  void check(){
    if(!_blocked)
      return;
    if(millis() > _whenBlocked + _duration)
      _blocked = false;
  }
  void block(){
    _blocked = true;
  }
  void unblock(){
    _blocked = false;
  }
  bool isBlocked(){
    return _blocked;
  }
};

class KeyCode{
private:
  static const byte _numRows = 4;
  static const byte _numCols = 3;
  char _keymap[_numRows][_numCols]= 
  {
    {'1', '2', '3'}, 
    {'4', '5', '6'}, 
    {'7', '8', '9'},
    {'*', '0', '#'}
  };
  byte _rowPins[_numRows] = {3,5,7,8};
  byte _colPins[_numCols]= {9,10,11};
  Keypad _keypad = Keypad(makeKeymap(_keymap), _rowPins, _colPins, _numRows, _numCols);
  
  int _passwordSize;
  char *_password;
  
  byte _currentIndex = 0;
  char *_attempt;

  Blocker *_blocker;

  bool _isSolved = false;
  void (*_onSuccess)();
  void (*_onFailure)();

  void _solve(){
    _isSolved = true;
    if(_onSuccess != 0L){
      _onSuccess();
    }
  }

  void _fail(){
    _currentIndex = 0;
    if(_onFailure != 0L){
      _onFailure();
    }
  }

  bool _attemptSuccessful(){
    bool result = true;
    
    for(int i=0; i<_passwordSize; i++)
      result = result && _attempt[i] == _password[i];

    return result;
  }
  
public:
  KeyCode(int passwordSize, char password[]){
    _passwordSize = passwordSize;
    _password = new char[passwordSize];
    _attempt = new char[passwordSize];
    _blocker = new Blocker();
    for(int i=0; i<passwordSize; i++){
      _password[i] = password[i];
      _attempt[i] = ' ';
    }
  }
  
  void check(){
    _blocker->check();

    if(_blocker->isBlocked())
      return; 
    if(_isSolved)
      return;
    
    char key = _keypad.getKey();
    if(key != NO_KEY){
      _attempt[_currentIndex] = key;
      _currentIndex++;
      
      if(_currentIndex == _passwordSize)
        if(_attemptSuccessful())
          _solve();
        else
          _fail();
    }
  }
  
  bool isSolved(){
    return _isSolved;
  }
  void drop(){
    _isSolved = false;
    _currentIndex = 0;
  }
  void restart(){
    if(!_isSolved)
      _currentIndex = 0;
  }
  void onSuccess(void (*callback)()){
    _onSuccess = callback;
  }
  void onFailure(void (*callback)()){
    _onFailure = callback;
  }
  void deactivate(long milliseconds){
    _blocker->block(milliseconds);
  }
  void deactivate(){
    _blocker->block();
  }
  void activate(){
    _blocker->unblock();
  }
};

#endif
