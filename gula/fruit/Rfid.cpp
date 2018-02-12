#ifndef RFID
#define RFID

#include <rdm630.h>

class Rfid {
  private:

    rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6

    void (*_onActive)(unsigned long uid);

    bool _isRfidAvailable() {
      return _rfid->available();
    }

    unsigned long _getUid() {
      byte data[6];
      byte length;

      _rfid->getData(data, length);
      unsigned long result =
        ((unsigned long int)data[1] << 24) +
        ((unsigned long int)data[2] << 16) +
        ((unsigned long int)data[3] << 8) +
        data[4];
      Serial.print("Rfid: ");
      Serial.println(result);
      return result;
    }

    void _activate(unsigned long uid) {
      if (_onActive != 0L) {
        _onActive(uid);
      }
    }


  public:
    Rfid() {
      _onActive = 0L;
      _rfid->begin();
    }

    void check() {
      if (_isRfidAvailable()) {
        unsigned long uid = _getUid();
        _activate(uid);
      }
    }

    void onActive(void (*callback)(unsigned long uid)) {
      _onActive = callback;
    }
};

#endif

