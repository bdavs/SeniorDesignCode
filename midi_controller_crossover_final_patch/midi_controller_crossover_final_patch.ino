/*

  converting the button presses from the clavier into MIDI signals

  Andrew Mele and Robert Davis
  2017-5-16

*/
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MCP23S17.h>

#define DEBOUNCE_DELAY  20    //debounce delay

MCP chip1(0, 9);              //instatntiate the two IC expander cops
MCP chip2(1, 10);

bool flags[28];
bool timerflags[28];
double timers[28];

//notes to be played by buttons E-flat3-G4
//Final Durfee Bell Tower
int notes[] = {70, 68, 67, 65, 63,
               62, 61, 60, 58, 57,
               56, 55, 53, 51
              };

//C4-B5 natural
/*int notes[] = {71, 69, 67, 65, 64,
                62, 60, 59, 57, 55,
                53, 52, 50, 48};*/
//bytes for each chip value
uint16_t shiftReg1;
uint16_t shiftReg2;
//uint32_t fullReg;

//create MIDI Instance
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midiOut);

void setup() {
  Serial.begin(31250);

  chipSetup(chip1);
  chipSetup(chip2);

  delay(25); //ensure set up in time

  //change instrument
  midiOut.sendProgramChange(14, 1);

  //setup flags
  for (int i = 0; i <= 27; i++)
  {
    timerflags[i] = true;

    if (i % 2 == 0)
      flags[i] = false;
    else
      flags[i] = true;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  shiftReg1 = buttonsPressed(chip1, shiftReg1);
  shiftReg2 = buttonsPressed(chip2, shiftReg2);
  //fullReg = ((uint32_t)shiftReg1 << 16) + (uint32_t)shiftReg2;//unused

  //This for-loop steps through the byte
  //bit by bit which holds the shift register data
  //and if it was high (1) then it prints
  //the corresponding location in the array

  for (int n = 0; n <= 15; n++)
  {
    if ((n % 2 == 0) && (shiftReg1 & (1 << n)) && (shiftReg1 & (1 << n + 1))) //if button is even and both the even button and it's odd counterpart are pressed
    {
      midiOut.sendNoteOff(notes[n / 2], 127, 1);
    }

    else if ((n % 2 != 0) && (shiftReg1 & (1 << n)) && (shiftReg1 & (1 << n - 1))) // if button is odd and both the odd button and it's even counterpart are pressed
    {
      midiOut.sendNoteOff(notes[(n - 1) / 2], 127, 1);
    }

    else if (shiftReg1 & (1 << n))
    {
      button_controller(n);
    }

    else if ( (n % 2 == 0) && !(shiftReg1 & (1 << (n + 1))) && timerflags[n] )
    {
      timers[n] = millis();
      timerflags[n] = false;
    }
  }

  for (int n = 0; n <= 11; n++ )
  {

    if ((n % 2 == 0) && (shiftReg2 & (1 << n)) && (shiftReg2 & (1 << n + 1))) //if button is even and both the even button and it's odd counterpart are pressed
    {
      midiOut.sendNoteOff(notes[n / 2], 127, 1);
    }

    else if ((n % 2 != 0) && (shiftReg2 & (1 << n)) && (shiftReg2 & (1 << n - 1))) // if button is odd and both the odd button and it's even counterpart are pressed
    {
      midiOut.sendNoteOff(notes[(n - 1) / 2], 127, 1);
    }

    else if (shiftReg2 & (1 << n))
    {
      button_controller(n + 16);
    }

    else if ((n % 2 == 0) && !(shiftReg2 & (1 << (n + 1))) && timerflags[n + 16])
    {
      timers[n + 16] = millis();
      timerflags[n + 16] = false;
    }
  }
}

//button controller function which sends appropriate
//MIDI command to Pi
void button_controller(int n)
{
  byte Velocity;  //final velocity byte sent to MidiController
  double delta;   //timer difference

  //even button was pressed, so this is note on
  if (n % 2 == 0)
  {
    if (flags[n])
    {
      flags[n + 1] = true;
      flags[n] = false;
      midiOut.sendNoteOff(notes[n / 2], 127, 1);
    }
    timerflags[n] = true;
  }
  //odd button was pressed, so this is note off
  else
  {
    if (flags[n])
    {
      timers[n] = millis();
      delta = timers[n] - timers[n - 1];

      //create maximum value for delta
      if (delta > 1500)
        delta = 1500;

      //map data into MIDI format
      Velocity = map(delta, 0, 1500, 100, 30);

      flags[n] = false;
      flags[n - 1] = true;
      midiOut.sendNoteOn(notes[(n - 1) / 2], Velocity, 1);
    }
  }
}

/*
   set up an mcp23s17
*/
void chipSetup(MCP chip) {
  chip.begin();
  //  chip.pinMode(0xFFFF);
  chip.wordWrite(0x00, 0xffff);

  //chip.pullupMode(0xFFFF);
  chip.wordWrite(0x0c, 0xffff);

  chip.inputInvert(0xFFFF);
}
/*
   read and debounce all the buttons on an mcp23s17
   returns the previous responce if anything goes wrong
*/
uint16_t buttonsPressed(MCP chip, uint16_t value2) {
  uint16_t value;
  value = chip.digitalRead();
  if (value != value2) { //make sure the value has changed
    delay(DEBOUNCE_DELAY);          //debounce delay
    if (value == chip.digitalRead())  //make sure the value didnt change again.
      return (value);
    else
      return (value2);
  }
  else
    return (value2);

}
