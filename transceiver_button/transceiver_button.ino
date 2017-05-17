/*
  designed for atmega328p
  
  Sending one command from a pushbutton to the xbee

   Robert Davis
   2017-5-17
*/
#include <SoftwareSerial.h>

#define xbeeRX A2
#define xbeeTX A3

#define midiRX A1
#define midiTX A0

#define TST1 3
#define TST2 4

bool flag = false;

SoftwareSerial xbeeSerial(xbeeRX, xbeeTX); // RX, TX

void setup() {
  xbeeSerial.begin(38400);
  pinMode(TST1, OUTPUT);
  digitalWrite(TST1, LOW);
  pinMode(TST2, INPUT_PULLUP);
}

/*
  If button is hit and wasn't hit in the past three seconds
  send a message to play the lowet bell three times in rapid succession

  the system will ignore any extra commands but will wake it up out of
  passthrough mode
*/
void loop() {
  //            note on    pitch     velocity
  byte mesg[] = {0x90,     0x3C,      0x78};
  //byte mesg[] = {0x90,     0x30,      0x78}; //backup command

  if (digitalRead(TST2) == LOW && flag == false) {
    xbeeSerial.write(mesg, sizeof(mesg));
    delay(1);
    xbeeSerial.write(mesg, sizeof(mesg));
    delay(1);
    xbeeSerial.write(mesg, sizeof(mesg));
    flag = true;
  }
  else if (digitalRead(TST2) == HIGH && flag == true) { //wait 3 seconds before allowing again
    delay(3000);
    flag = false;
  }


}
