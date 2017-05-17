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

   xbeeSerial.begin(38400);
   midiSerial.begin(31250);
   delay(20);
  xbeeSerial.write(0xC0);
  xbeeSerial.write(0x0E);

}

// the loop function runs over and over again forever
//simply reads the midi input and sends it as output over xbee
void loop() {
  
  while (midiSerial.available()){
    delay(1);//required to get all the data
   xbeeSerial.write(midiSerial.read());
  }

}
