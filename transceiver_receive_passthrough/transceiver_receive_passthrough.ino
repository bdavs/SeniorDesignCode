
/*
 * designed for atmega328p
 * 
 * Receiving from an xbee to the midi interface
 * with midi passthrough.
 * 
 * Robert Davis
 * 2017-4-14
 */
 
#include <SoftwareSerial.h>

#define DEBUG 1
#define SECONDS(sec) (1000 * (sec))

#define xbeeRX A2
#define xbeeTX A3

#define midiRX A1
#define midiTX A0

#define TST1 3
#define TST2 4


uint32_t timer = 0;

//#IF DEBUG
uint32_t timer2 = 0;
//#ENDIF

// temp variable to check if xbee is receiving 
bool xbeeRX_State = 0;

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
  /************************************
  //variable initialization
  *************************************/
  int cmd = 0;
  //int pitch = 0;
  //int velocity = 0;

  /***********************************
  //xbee receive block
  *************************************/
  if (xbeeSerial.isListening()) {//if xbeeserial port is listening
    if (xbeeSerial.available()) { //if there is data available
      delay(1); //required to get all the data
      cmd = xbeeSerial.read(); //read data in from xbee
      midiSerial.write(cmd); // write data into midi
      timer = millis(); //reset timer
    }
  }
  else if (digitalRead(xbeeRX) != xbeeRX_State) {
    xbeeRX_State = !xbeeRX_State;
    timer = millis(); //reset timer
  }

  /***********************************
  //print debug info
  *************************************/
//#IF DEBUG
  if ( (millis() - timer2) > SECONDS(10)) { //if the timer has been going for more than 10 seconds
    if (midiSerial.isListening())
      xbeeSerial.println("midi is listening");

    if (xbeeSerial.isListening())
      xbeeSerial.println("xbee is listening");
    timer2 = millis();
  }
//#ENDIF

  //***********************************
  //midi passthrough receive block
  //*************************************/
  if ( (millis() - timer) > SECONDS(10)) { //if the timer has been going for more than 10 seconds
    midiSerial.listen(); //begin listening on the midi port
    delay(2); //small delay to ensure it is listening
    if (midiSerial.available()) {//if there is data available
      delay(1); //delay to ensure data integrity
      cmd = midiSerial.read(); //read data from midi port
      midiSerial.write(cmd); //send data to midi out
    }
  }
  else { //timer is less than 10 seconds
    midiSerial.flush(); //delete anything in midi buffer 
    if (! xbeeSerial.isListening()) { //if the xbee isnt already listening
      xbeeSerial.listen(); //start listening
      delay(2); //small delay to ensure that the port is listening
    }
  }
}

