/*
  MCP23S17_buttons file
  Robert Davis 2017
  


This file contains two functions that will be useful for the chime simulator project
chipSetup will set up the mcp23S17 chip to work with our setup
buttonsPressed will return a 16bit word with each bit representing a single button
  this function also does some basic debouncing

*/

#include <SPI.h>              // We use this library, so it must be called here.
#include <MCP23S17.h>         // Here is the new class to make using the MCP23S17 easy.
MCP chip1(1,10);             // Instantiate an object called "chip1" on an MCP23S17 device at address 1
  								     // and slave-select on Arduino pin 10
MCP chip2(0,9);
byte addr =0x0;
#define DEBOUNCE_DELAY  20    //debounce delay

void chipSetup(MCP chip){
  chip.begin();
//  chip.pinMode(0xFFFF);     
  chip.wordWrite(0x00,0xffff);
  
  //chip.pullupMode(0xFFFF);  
  chip.wordWrite(0x0c,0xffff);

  chip.inputInvert(0x0000); 
  //chip.wordWrite(
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



//*****BEGIN EXAMPLE CODE*****//   
//setup chip, say hello, print out chip reading every 100(+20) ms                    
void setup() {
  chipSetup(chip1);
  chipSetup(chip2);
  //pinMode(9,OUTPUT);
  //digitalWrite(9,HIGH);
  Serial.begin(9600);
  Serial.println("hello");
}

void loop() {
  
   uint16_t value, value2;       
   uint32_t final;   
                  
   value = buttonsPressed(chip1,value);//chip1.digitalRead(4);//
   value2 = buttonsPressed(chip2,value2);
   final = ((uint32_t)value << 16) + (uint32_t)value2;
   //final = final <<16;
   Serial.println(final,BIN);
   //Serial.println(value2,BIN);
   delay(100);
   //*/
/*
   byte output;
   output = chip1.byteRead(addr);
   Serial.print(addr,HEX);
   Serial.print(": ");
   Serial.println(output, BIN);
   addr++;
*/
}

//*****END EXAMPLE CODE*****//  


