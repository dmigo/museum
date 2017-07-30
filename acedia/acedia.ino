#define DEBOUNCE_TIME 100
#define FRAGMENT_DURATION 1000
#define COMPOSITION_DURATION 10000

class Btn{//use inheritance
  private:
  int _state = HIGH;
  long _stateTimestamp = 0;
  int _pin;
  int _fragment;
  int _duration;
  
  bool _stateChanged(){
    return digitalRead(_pin) != _state;
  }
  bool _debounced(){
    return millis() > _stateTimestamp + DEBOUNCE_TIME;
  }
  
  public:
  Btn(int pin, int fragment, int duration){
    _pin = pin;
    _fragment = fragment;
    pinMode(pin, INPUT_PULLUP);
  }
  
  void check(){
    if(_stateChanged()
      && _debounced()){
        _state = digitalRead(_pin);
        _stateTimestamp = millis();
        
        if(_state == LOW){
          Serial.println();
          Serial.print(_pin);
          Serial.print(": pressed");
          //Todo player.play(fragment);
          delay(_duration);
        }else{
          Serial.println();
          Serial.print(_pin);
          Serial.print(": released");
        }
      }
  }
};

class BtnSequence{
  private:
  static const int _btnAmount = 6;
  const int _sequence[_btnAmount] = {0,1,2,3,4,5};
  Btn _buttons[6] = {
    new Btn(1, 1, FRAGMENT_DURATION),
    new Btn(2, 2, FRAGMENT_DURATION),
    new Btn(3, 3, FRAGMENT_DURATION),
    new Btn(4, 4, FRAGMENT_DURATION),
    new Btn(5, 5, FRAGMENT_DURATION),
    new Btn(6, 6, FRAGMENT_DURATION)};
  public:
  Sequence(){
  }

  bool completed(){
    bool isCompleted = false;
    
    if(isCompleted)
      Serial.println("sequence completed");
    return isCompleted;
  }
};

class Lock{
  private:
  int _pin;
  bool _locked = false;
  
  public:
  Lock(int pin){
    _pin = pin;
    pinMode(pin, OUTPUT);
  }

  void lock(){
    if(!_locked){
      digitalWrite(_pin, LOW);
      _locked = true;
      Serial.println();
      Serial.print(_pin);
      Serial.print("locked");
    }
  }

  void unlock(){
    if(_locked){
      digitalWrite(_pin, HIGH);
      _locked = false;
      Serial.println();
      Serial.print(_pin);
      Serial.print("unlocked");
    }
  }
};

BtnSequence sequence;
Lock lock;
const int lockPin = 10;
Btn compositionBtn;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
  
  // put your setup code here, to run once:
  sequence = new BtnSequence();
  lock = new Lock(lockPin);
  lock->lock();
  compositionButton = new Btn(10, 10, COMPOSITION_DURATION);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(sequence.completed())
    lock.unlock();
}
