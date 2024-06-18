#include <TM1637Display.h>
#define CLK 2
#define DIO 3
TM1637Display display(CLK, DIO);
const int red = 12;
const int green = 13;
#define BUTTON_PIN 4 // Pin for the pushbutton
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isRunning = false;
bool isBroken = false;

void setup() {

  Serial.begin(9600);
  display.setBrightness(5);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(green, 1);
  digitalWrite(red, 1);
}

void loop() 
if (Serial.available() > 0) {
    char command = Serial.read();
//    if (command == 's')
//    {
//      digitalWrite(green, 0);
//      digitalWrite(red, 1);
//    }
    if (command == 's') {
      if (!isRunning) {
        if (isBroken) {
          // Restart the timer if it was previously broken
          startTime = millis() - elapsedTime;
          isBroken = false;
        } else {
          // Start the timer
          startTime = millis(); // Record the start time only if the timer is not running
        }
        isRunning = true;
      } else {
        // Break the running time if already running
        elapsedTime = millis() - startTime;
        isBroken = true;
        isRunning = false;
      }
    } else if (command == 'r') {
      // Reset the timer
      isRunning = false;
      isBroken = false;
      elapsedTime = 0;
    }
  }
  
  // Check if the pushbutton is pressed to break the time
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (isRunning) {
      // Break the running time
      
      elapsedTime = millis() - startTime;
      isBroken = true;
      isRunning = false;
    }
  }
  
  if (isRunning) {
    elapsedTime = millis() - startTime;
    displayTime(elapsedTime); // Display elapsed time in milliseconds and seconds
  } else {
    displayTime(elapsedTime); // Display elapsed time even when paused or broken
  }
}

void displayTime(unsigned long milliseconds) {
  // Calculate seconds and milliseconds
  unsigned long seconds = milliseconds / 1000;
  milliseconds %= 1000;
  
  // Format the time as ss:ms on the display
  byte data[] = {
    display.encodeDigit(seconds / 10),   // Tens of seconds
    display.encodeDigit(seconds % 10),   // Units of seconds
    display.encodeDigit(milliseconds / 100), // Hundreds of milliseconds
    display.encodeDigit((milliseconds / 10) % 10) // Tens of milliseconds
  };
  
  display.setSegments(data);
}
