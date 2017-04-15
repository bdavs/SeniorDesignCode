
/*
Receiving from an xbee to the midi interface
with midi passthrough.
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

uint32_t timer = 0, timer2 = 0;
bool state = 0;

SoftwareSerial midiSerial(midiRX, midiTX); // RX, TX
SoftwareSerial xbeeSerial(xbeeRX, xbeeTX); // RX, TX



void setup() {
  midiSerial.begin(31250); //
  xbeeSerial.begin(38400);
  //  attachInterrupt(xbeeRX, xbeeInt, CHANGE);
  xbeeSerial.write("hello");
  //xbeeSerial.write("sent");
  timer =millis();
}

// the loop function runs over and over again forever
void loop() {
  int cmd = 0;
  //int pitch = 0;
  //int velocity = 0;

  if (xbeeSerial.available()) {
    delay(1);//required to get all the data
    cmd = xbeeSerial.read();
    midiSerial.write(cmd);
    timer = millis();
  }
  else if (digitalRead(xbeeRX) != state) {
    state = !state;
    timer = millis();
  }
  
#IF DEBUG
  if ( (millis() - timer2) > SECONDS(10)) {
    if (midiSerial.isListening())
      xbeeSerial.println("midi is listening");

    if (xbeeSerial.isListening())
      xbeeSerial.println("xbee is listening");
    timer2 = millis();
  }
#ENDIF

  if ( (millis() - timer) > SECONDS(10)) { //10000){
    midiSerial.listen();
    delay(2);
    if (midiSerial.available()) {
      delay(1);
      cmd = midiSerial.read();
      midiSerial.write(cmd);
      // xbeeSerial.write(cmd);
    }
  }
  else { // if (midiSerial.available() && (millis() - timer) < 1000){
    midiSerial.flush();
    if (! xbeeSerial.isListening()) {
      xbeeSerial.listen();
      delay(2);
    }
  }
}

//void xbeeInt() {
  //xbeeSerial.listen();
  //delay(1);
  //xbeeSerial.write("interrupt");
  //timer = millis();
//}



