#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <SoftwareSerial.h>
#include <SPI.h>              // We use this library, so it must be called here.
#include <MCP23S17.h>         // Here is the new class to make using the MCP23S17 easy.
MCP chip2(1, 10);             // Instantiate an object called "chip1" on an MCP23S17 device at address 1
MCP chip1(0, 9);             // and slave-select on Arduino pin 10
#define DEBOUNCE_DELAY  20    //debounce delay
SoftwareSerial mySerial(A0,A1); //RX TX

bool flags[32];
bool timerflags[32];
double timers[32];
//notes to be played by buttons C4-B5
int notes[] = {48, 50, 52, 53, 55, 
                57, 59, 60, 62, 64, 
                65, 67, 69, 71};
//bytes for each chip value
uint16_t shiftReg1;
uint16_t shiftReg2;
uint32_t fullReg;

//create MIDI Instance
//MIDI_CREATE_INSTANCE(SoftwareSerial,mySerial, midiOut);
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //mySerial.begin(31250);
  //pinMode(9,OUTPUT);
  //digitalWrite(9,HIGH);
  
  MIDI.begin(1);
  chipSetup1(chip1);
  chipSetup2(chip2);

  delay(25);
  //change instrument
  MIDI.sendProgramChange(14,1);
  
  //setup flags
  for (int i = 0; i < 32; i++)
  {
    timerflags[i] = true;
    
    if (i % 2 == 0)
      flags[i] = false;
    else
      flags[i] = true;
  }

//  counter = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
                        
   shiftReg1 = buttonsPressed(chip1,shiftReg1);
   shiftReg2 = buttonsPressed(chip2,shiftReg2);
   fullReg = ((uint32_t)shiftReg1 << 16) + (uint32_t)shiftReg2;
  //This for-loop steps through the byte
  //bit by bit which holds the shift register data 
  //and if it was high (1) then it prints
  //the corresponding location in the array
  for (int n=0; n<=27; n++)
  {
    if (fullReg & (1 << n))
    {
      button_controller(n);
    }
   
    else if ( (n % 2 == 0) && !(fullReg & (1<<(n+1))) && timerflags[n] )
    {
      timers[n] = millis();
      timerflags[n] = false;
    }
    /*if (shiftReg2 & (1 << n))
    {
      button_controller(n+16);
    }*/

  }
  //counter++;
}


//button controller function which sends appropriate
//MIDI command to Pi

void button_controller(int n)
{
  byte Velocity;
  double delta;
  
  //even button was pressed, so this is note on
  if (n % 2 == 0)
  {
    if(flags[n])
    {
      flags[n+1] = true;
      flags[n] = false;
      MIDI.sendNoteOff(notes[n/2], 127, 1);
    } 
    timerflags[n] = true;
  }
  //odd button was pressed, so this is note off
  else
  {
    if(flags[n])
    {
      timers[n] = millis();
      delta = timers[n] - timers[n-1];
      
      //create maximum value for delta
      if (delta > 1500)
        delta = 1500;
        
      //map data into MIDI format
      Velocity = map(delta, 0, 1500, 100, 30);
      
      flags[n] = false;
      flags[n-1] = true;
      MIDI.sendNoteOn(notes[(n-1)/2], Velocity, 1);
     /* Serial.print("Timer0: ");
      Serial.println(timers[n-1]);
      Serial.print("Timer1: ");
      Serial.println(timers[n]);
      Serial.print("Time Difference: ");
      Serial.println(delta,DEC);
      */
      //Serial.print("Velocity Map: "); 
      //Serial.println(Velocity,DEC);
    }
  }  
}

void chipSetup1(MCP chip){
  chip.begin();
//  chip.pinMode(0xFFFF);     
  chip.wordWrite(0x00,0xffff);
  
  //chip.pullupMode(0xFFFF);  
  chip.wordWrite(0x0c,0xffff);

  chip.inputInvert(0x0000); 
}
void chipSetup2(MCP chip){
  chip.begin();
//  chip.pinMode(0xFFFF);     
  chip.wordWrite(0x00,0xffff);
  
  //chip.pullupMode(0xFFFF);  
  chip.wordWrite(0x0c,0xffff);

  chip.inputInvert(0x0000); 
}

uint16_t buttonsPressed(MCP chip, uint16_t value2){
  uint16_t value;
  value = chip.digitalRead();
  if (value != value2){ //make sure the value has changed
    delay(DEBOUNCE_DELAY);          //debounce delay
    if (value == chip.digitalRead())  //make sure the value didnt change again.
      return(value);
    else
       return(value2);
   }
   else
     return(value2);
  
}
