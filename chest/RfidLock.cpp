#ifndef RFID_LOCK
#define RFID_LOCK

#include <rdm630.h>

class RfidLock{
private:

  rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6

  unsigned long _key;
  bool _isOpen = false;
  void (*_onActive)(unsigned long uid);
  
  bool _isRfidAvailable(){
    return _rfid->available();
  }
  
  unsigned long _getUid(){
    byte data[6];
    byte length;
    
    _rfid->getData(data, length);
    unsigned long result = 
      ((unsigned long int)data[1]<<24) + 
      ((unsigned long int)data[2]<<16) + 
      ((unsigned long int)data[3]<<8) + 
      data[4];
  
    return result;
  }

  void _activate(unsigned long uid){
      if (_onActive != 0L) {
        _onActive(uid);
      }
  }
  
  
public:
  RfidLock(unsigned long key){
    _onActive = 0L;
    _rfid->begin();
    _key = key;
  }
  
  void check(){
    if(_isRfidAvailable()){
      unsigned long uid = _getUid();
      _activate(uid);

      _isOpen = _isOpen || uid == _key;
    }
  }

  void onActive(void (*callback)(unsigned long uid)){
    _onActive = callback;
  }
  
  bool isOpen(){
    return _isOpen;
  }

  void drop(){
    _isOpen = false;
  }
};

#endif

