#include <TM1637Display.h>
#define CLK 2
#define DIO 3
TM1637Display display(CLK, DIO);
void setup() {

  display.setBrightness(5);

}

void loop() {

  

  display.clear();

  for(int i=0; i <= 9999; i++)
  {
    display.showNumberDec(i,true);
    delay(100);
  }
delay(1000);
}

/******************************************************************************
 * Interfacing TM1637 4-Digit seven segment display module with Arduino.
 * This example shows how print an incremented number on the TM1637 display
       from 0 to 9999.
 * This is a free software with NO WARRANTY.
 * http://simple-circuit.com/
/*******************************************************************************/

//// include TM1637 display module library
//#include <TM1637Display.h>
//
//// define TM1637 Module connection pins
//#define CLK 3   // clock pin
//#define DIO 2   // data pin
//
//// initialize TM1637 display module library
//TM1637Display display(CLK, DIO);
//
//void setup() {
//  // set display brightness (7 maximum brightness and 0 minimum)
//  display.setBrightness(7);
//}
//
//uint16_t i = 0;
//void loop() {
//
//  // print variable 'i' on the display (decimal format with leading zeros)
//  display.showNumberDec(i, true);
//
//  i++;           // increment 'i'
//  if(i > 9999)
//    i = 0;
//
//  delay(1000);
//
//}

// end of code.
