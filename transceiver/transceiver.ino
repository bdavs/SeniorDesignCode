/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#include <SoftwareSerial.h>

//SoftwareSerial midiSerial(A1, A0); // RX, TX
SoftwareSerial xbeeSerial(A2, A3); // RX, TX

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  //pinMode(4, OUTPUT); //TST2
  //pinMode(8,INPUT);
  //midiSerial.begin(31250);
  xbeeSerial.begin(9600);
   
}

// the loop function runs over and over again forever
void loop() {
  int cmd = 0;
  int pitch = 0;
  int velocity = 0;
  //delay(10000);
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

    
    /*
    digitalWrite(4,HIGH);
    delay(1000);
    digitalWrite(4,LOW);
    delay(1000);
    */
   xbeeSerial.write(xbeeSerial.read());
      // turn the LED on (HIGH is the voltage level)
    //delay(500);              // wait for a second
    //digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    //delay(1000);              // wait for a second
  }
  //digitalWrite(13, LOW);
}
