#ifndef Lock_H
#define Lock_H

class Lock{
  private:
  int _pin;
  bool _closed;
  
  public:
  Lock(int pin);
  void lock();
  void unlock();
};

#endif
