/*
Sending from a midi controller to the xbee
 */
#include <SoftwareSerial.h>

#define xbeeRX A2
#define xbeeTX A3

#define midiRX A1
#define midiTX A0


//SoftwareSerial midiSerial(A1, A0); // RX, TX //may swap these pins?
SoftwareSerial xbeeSerial(xbeeRX, xbeeTX); // RX, TX

// the setup function runs once when you press reset or power the board
  // initialize digital pin 13 as an output.
  //pinMode(4, OUTPUT); //TST2
  //midiSerial.begin(31250);
  xbeeSerial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  int cmd = 0;
  int pitch = 0;
  int velocity = 0;

  if (xbeeSerial.available()){
    /*
    cmd = Serial.read();
    pitch = Serial.read();
    velocity = Serial.read();
  //}
  //if(pitch>0x40&&pitch<0x60){
     digitalWrite(13, HIGH);
    mySerial.write(cmd);
    mySerial.write(pitch);
    mySerial.write(velocity);
    */
   xbeeSerial.write(xbeeSerial.read());

  }
}
