#include "rdm630.h"

class RfidLock{
private:
  rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
  unsigned long _key;
  bool _isOpen = false;
  
  bool _isRfidAvailable(){
    return _rfid->available();
  }
  int _getRfid(){
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
  
public:
  RfidLock(unsigned long key){
    _rfid->begin();
    _key = key;
  }
  
  void check(){
    if(_isOpen)
      return;
      
    if(_isRfidAvailable()){
      Serial.println("Data available");
      long result = _getRfid();
      Serial.print("Rfid: ");
      Serial.println(result);
      _isOpen = result == _key;
    }
  }
  
  bool isOpen(){
    return _isOpen;
  }
};
