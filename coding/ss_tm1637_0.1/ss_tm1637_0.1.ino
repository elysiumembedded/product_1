#include <TM1637Display.h>
#include <SoftwareSerial.h>

#define CLK 2    // CLK pin of the TM1637 connected to Arduino digital pin 2
#define DIO 3    // DIO pin of the TM1637 connected to Arduino digital pin 3
#define BUTTON_PIN 4 // Pin for the pushbutton
#define red 11
#define green 12
#define buzzer 10

TM1637Display display(CLK, DIO);
SoftwareSerial bluetoothSerial(7,8);

unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isRunning = false;
bool isStandby = false;

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); // Built-in LED for indication
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Pushbutton pin
  display.setBrightness(0x0f); // set the brightness to maximum
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  digitalWrite(buzzer, LOW);
}

void loop() {
  if (bluetoothSerial.available() > 0) {
    char command = bluetoothSerial.read();
    if (command == '1') {
      digitalWrite(red, HIGH);
      digitalWrite(green, LOW);
      if (!isRunning && !isStandby) {
        // Start the timer
        startTime = millis(); 
        isRunning = true;
      } else if (isRunning && !isStandby) {
        // Stop the timer
        elapsedTime = millis() - startTime;
        isRunning = false;
      }
    } else if (command == '2') {
      // Reset the timer
      isRunning = false;
      elapsedTime = 0;
    }
  }

  // Check if the pushbutton is pressed to toggle standby mode
  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(buzzer, HIGH);
    isStandby = !isStandby;
    delay(10000); // Debounce delay
  }

  // If timer is in standby mode, turn off the display
  if (isStandby) {
    display.setBrightness(0);
    return; // Exit the loop, nothing else to do in standby mode
  } else {
    // If not in standby mode, display the elapsed time
    display.setBrightness(0x0f); // Restore display brightness
    if (isRunning) {
      elapsedTime = millis() - startTime;
      displayTime(elapsedTime);
    } else {
      displayTime(elapsedTime);
    }
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
