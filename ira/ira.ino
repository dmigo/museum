#include <Lock.h>
#include <Rfid.h>

#define UNLOCK_DELAY 10000 //задержка для замка

#define SS_PIN 10
#define RST_PIN 9

#define SCL A5
#define SDA A4

#define LOCK_PIN 2

#define UID 19001  //айдишник нужной нфцшки

Rfid *rfid;
//Lock *lock;

void setup() {
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  

  Serial.println("Initializing...");
 // lock = new Lock(LOCK_PIN);
  rfid = new Rfid(SS_PIN, RST_PIN);

 // lock->lock();
  Serial.println("Start...");
}

void loop() {
  int uid = rfid->readUid();
  
//  if(uid == UID){
    //Serial.println("Unlocking!");
    //lock->unlock();
    //delay(UNLOCK_DELAY);
    //Serial.println("Locking!");
    //lock->lock();
    //delay(UNLOCK_DELAY);
  //}
delay(1000);
 // if(uid!=0){
    Serial.print("Current uid is: ");
    Serial.println(uid);
  //}
}

