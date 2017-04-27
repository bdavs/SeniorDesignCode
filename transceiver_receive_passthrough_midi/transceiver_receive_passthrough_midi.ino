
/*
 * designed for atmega328p
 *
 * Receiving from an xbee to the midi interface
 * with midi passthrough.
 *
 * Robert Davis
 * 2017-4-14
 */
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
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

//create MIDI Instance
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, midiInOut);
MIDI_CREATE_INSTANCE(SoftwareSerial, xbeeSerial, xbeeIn);

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
  if (xbeeSerial.isListening()) {//if xbeeSerial port is listening
    //#IF DEBUG
    if (xbeeIn.read())                // Is there a MIDI message incoming ?
    {
      switch (xbeeIn.getType())     // Get the type of the message we caught
      {
        case midi::ProgramChange:       // If it is a Program Change,
          xbeeSerial.write("poop");
        case midi::NoteOn:
          midiInOut.send(xbeeIn.getType(),
                         xbeeIn.getData1(),
                         xbeeIn.getData2(),
                         xbeeIn.getChannel());
      }
    }
    //#ELSE
    /*
        if (xbeeSerial.available()) { //if there is data available
          delay(1); //required to get all the data
          cmd = xbeeSerial.read(); //read data in from xbee
          midiInOut.
          midiSerial.write(cmd); // write data into midi
          timer = millis(); //reset timer
        }
    //#ENDIF
    */
  }
  else if (digitalRead(xbeeRX) != xbeeRX_State) {
    xbeeRX_State = !xbeeRX_State;
    timer = millis(); //reset timer
  }

  /***********************************
  //print debug info
  *************************************/
  /*#IF DEBUG*/
    if ( (millis() - timer2) > SECONDS(10)) { //if the timer has been going for more than 10 seconds
      if (midiSerial.isListening())
        xbeeSerial.println("\nmidi is listening");

      if (xbeeSerial.isListening())
        xbeeSerial.println("\nxbee is listening");
      timer2 = millis();
    }
    /*
  #ENDIF
  */
  //***********************************
  //midi passthrough receive block
  //*************************************/
  if ( (millis() - timer) > SECONDS(10)) { //if the timer has been going for more than 10 seconds
    midiSerial.listen(); //begin listening on the midi port
    delay(2);
    if (midiInOut.read())                // Is there a MIDI message incoming ?
    {
      switch (midiInOut.getType())     // Get the type of the message we caught
      {
        case midi::ProgramChange:       // If it is a Program Change,
          //xbeeSerial.write("poop");
          midiInOut.send(midiInOut.getType(),
                         midiInOut.getData1(),
                         midiInOut.getData2(),
                         midiInOut.getChannel());
        case midi::NoteOn:
          midiInOut.send(midiInOut.getType(),
                         midiInOut.getData1(),
                         midiInOut.getData2(),
                         midiInOut.getChannel());
      }
    }
    /*
     //small delay to ensure it is listening
    if (midiSerial.available()) {//if there is data available
      delay(1); //delay to ensure data integrity
      cmd = midiSerial.read(); //read data from midi port
      midiSerial.write(cmd); //send data to midi out
      xbeeSerial.println("sending");
    }
    */
  }
  else { //timer is less than 10 seconds
    midiSerial.flush(); //delete anything in midi buffer
    if (! xbeeSerial.isListening()) { //if the xbee isnt already listening
      xbeeSerial.listen(); //start listening
      delay(2); //small delay to ensure that the port is listening
    }
  }
}

