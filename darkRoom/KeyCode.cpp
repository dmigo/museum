#include <Keypad.h>

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
  byte _rowPins[_numRows] = {12,11,10,9};
  byte _colPins[_numCols]= {8,7,6};
  Keypad _keypad= Keypad(makeKeymap(_keymap), _rowPins, _colPins, _numRows, _numCols);
  
  int _passwordSize;
  char *_password;
  
  byte _currentIndex = 0;
  char *_attempt;
  
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
    for(int i=0; i<passwordSize; i++){
      _password[i] = password[i];
      _attempt[i] = ' ';
    }
  }
  
  void check(){
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
  void onSuccess(void (*callback)()){
    _onSuccess = callback;
  }
  void onFailure(void (*callback)()){
    _onFailure = callback;
  }
};
