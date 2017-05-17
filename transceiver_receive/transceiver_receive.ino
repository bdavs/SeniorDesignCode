/*
Sending from a midi controller to the xbee
 */
#include <SoftwareSerial.h>

#define xbeeRX A2
#define xbeeTX A3

#define midiRX A1
#define midiTX A0

#define TST1 3
#define TST2 4


SoftwareSerial midiSerial(midiRX, midiTX); // RX, TX //may swap these pins?
SoftwareSerial xbeeSerial(xbeeRX, xbeeTX); // RX, TX
void setup(){

  //pinMode(4, OUTPUT); //TST2
/*pinMode(xbeeRX, INPUT); //TST2
pinMode(xbeeTX, INPUT); //TST2
  pinMode(TST2, OUTPUT);
 digitalWrite(TST2,HIGH);
  delay(50);
  digitalWrite(TST2,LOW); 
delay(50);
   digitalWrite(TST2,HIGH);
 // pinMode(xbeeTX, OUTPUT);
 // digitalWrite(xbeeTX,HIGH);
  */
   midiSerial.begin(31250);
   xbeeSerial.begin(38400);
  
  //xbeeSerial.write("hello");
   xbeeSerial.write("sent");

}

// the loop function runs over and over again forever
void loop() {
  int cmd = 0;
  int pitch = 0;
  int velocity = 0;

  while(xbeeSerial.available()){
    delay(1);//required to get all the data

    xbeeSerial.write("midisent");

   midiSerial.write(xbeeSerial.read());
  //}
 }

}
