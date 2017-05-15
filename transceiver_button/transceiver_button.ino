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

bool flag = false;

//SoftwareSerial midiSerial(midiRX, midiTX); // RX, TX
SoftwareSerial xbeeSerial(xbeeRX, xbeeTX); // RX, TX
void setup() {


  xbeeSerial.begin(38400);
  pinMode(TST1, OUTPUT);
  pinMode(TST2, INPUT_PULLUP);
  digitalWrite(TST1, LOW);
  delay(20);


}

// the loop function runs over and over again forever
void loop() {
  int noop = 0xAA;
  int cmd = 90;
  int pitch = 48;//60;
  int velocity = 120;
  //            note on    pitch     velocity
  byte mesg[] = {0x90,     0x3C,      0x78};
  

  if (digitalRead(TST2) == LOW && flag==false) {
    xbeeSerial.write(mesg, sizeof(mesg));
    delay(1);
    xbeeSerial.write(mesg, sizeof(mesg));
    delay(1);
    xbeeSerial.write(mesg, sizeof(mesg));
    flag=true;
  }
  else if(digitalRead(TST2) == HIGH && flag==true){
    delay(3000);
    //xbeeSerial.flush();
    flag=false;
  }


}
