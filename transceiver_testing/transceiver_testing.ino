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


SoftwareSerial midiSerial(midiRX, midiTX); // RX, TX 
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
   xbeeSerial.begin(38400);
   midiSerial.begin(31250);
  //xbeeSerial.write("hello");

}

// the loop function runs over and over again forever
void loop() {
  int cmd = 0;
  int pitch = 0;
  int velocity = 0;
  
  //xbeeSerial.print("hello");
  //delay(2000);

  while (midiSerial.available()){
    delay(1);//required to get all the data
    
    cmd = midiSerial.read();
    pitch = midiSerial.read();
    velocity = midiSerial.read();
  }
  //}
  //if(pitch>0x40&&pitch<0x60){
    // digitalWrite(13, HIGH);
   if(cmd==0x90&&pitch){
    xbeeSerial.write(cmd);
    //midiSerial.write(cmd);
    
    xbeeSerial.write(pitch);
   // midiSerial.write(pitch);
    
    xbeeSerial.write(velocity);
    //midiSerial.write(velocity);

    xbeeSerial.write("yay");

   //xbeeSerial.write(midiSerial.read());
  }
//*/
}
