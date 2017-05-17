
/*
   designed for atmega328p

   Receiving from an xbee to the midi interface
   with midi passthrough.

   Robert Davis
   2017-4-14
*/

#include <SoftwareSerial.h>

#define SECONDS(sec) (1000 * (sec))

#define xbeeRX A2
#define xbeeTX A3

#define midiRX A1
#define midiTX A0

#define TST1 3
#define TST2 4

uint32_t timer = 0;

//initialize software serial ports for passthrough and wireless
SoftwareSerial midiSerial(midiRX, midiTX); // RX, TX
SoftwareSerial xbeeSerial(xbeeRX, xbeeTX); // RX, TX

void setup() {
  midiSerial.begin(31250); //midi operates at 31250 BAUD and must be set to this
  xbeeSerial.begin(38400); //xbee must operate at least 31250 and 38400 has been the most stable
  //xbeeSerial.write("hello"); //bootup test
  timer = millis(); //timer start
}

// the loop function runs over and over again forever
void loop() {
  /*
     xbee receive block
  */
  if (xbeeSerial.isListening()) {//if xbeeSerial port is listening
    //delay(2);
    while (xbeeSerial.available()) {
      delay(1);
      midiSerial.write(xbeeSerial.read());
      timer = millis();
    }
  }
  /*
     switch to xbee receive
  */
  else if (digitalRead(xbeeRX) == LOW) { 
    timer = millis(); //reset timer
    midiSerial.flush(); //delete anything in midi buffer
    if (! xbeeSerial.isListening()) { //if the xbee isnt already listening
      xbeeSerial.listen(); //start listening
      //delay(2); //small delay to ensure that the port is listening
    }
  }

  /*
    midi passthrough receive block
  */
  if ( (millis() - timer) > SECONDS(10)) { //if the timer has been going for more than 10 seconds
    if (!midiSerial.isListening()) {
      midiSerial.listen(); //begin listening on the midi port
      delay(1);
    }
    if (midiSerial.available()) {
      midiSerial.write(midiSerial.read());
    }

  }
}

