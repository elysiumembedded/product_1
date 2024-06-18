#include <LedControl.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int DIN_PIN = 4;
const int CS_PIN = 3;
const int CLK_PIN = 2;
const int LIMIT_SWITCH_PIN = 5; // Limit switch pin
const int red = 11;
const int green = 12;
const int buzzer = 9;

// Create a LedControl object
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); // DIN, CLK, CS, number of displays

// Create a SoftwareSerial object for Bluetooth communication
SoftwareSerial bluetooth(7, 8);

// Variables for stopwatch
unsigned long previousMillis = 0;
unsigned long elapsedMillis = 0;
bool running = false;
bool paused = false;
bool limitSwitchPressed = false;

void setup() {
  // Initialize the MAX7219
  lc.shutdown(0, false);       // Wake up the MAX7219
  lc.setIntensity(0, 8);       // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);          // Clear display register
  
  // Initialize Bluetooth communication
  bluetooth.begin(9600);

  // Initialize limit switch pin
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(green, HIGH);
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Stopwatch Initialized. Enter 'start', 'stop', or 'reset' to control the stopwatch.");
}

void loop() {
  // Check for serial input from Bluetooth
  if (bluetooth.available() > 0) {
    String command = bluetooth.readStringUntil('\n');
    command.trim(); // Remove any leading/trailing whitespace
   
    if (command == "start") {
      if (!running) {
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
        running = true;
        previousMillis = millis();
        Serial.println("Stopwatch started.");
        bluetooth.println("Stopwatch started.");
      }
    } else if (command == "stop") {
      if (running) {
        running = false;
        Serial.println("Stopwatch stopped.");
        bluetooth.println("Stopwatch stopped.");
      }
    } else if (command == "reset") {
      elapsedMillis = 0;
      running = false;
      lc.clearDisplay(0);
      Serial.println("Stopwatch reset.");
      bluetooth.println("Stopwatch reset.");
    } else {
      Serial.println("Unknown command. Please enter 'start', 'stop', or 'reset'.");
      bluetooth.println("Unknown command. Please enter 'start', 'stop', or 'reset'.");
    }
  }

  // Check limit switch state
  if (digitalRead(LIMIT_SWITCH_PIN) == LOW) {
    if (!limitSwitchPressed) {
      digitalWrite(buzzer, HIGH);
      limitSwitchPressed = true;
      if (running) {
        running = false;
        Serial.println("Stopwatch stopped by limit switch.");
        bluetooth.println("Stopwatch stopped by limit switch.");
      }
    }
  } else {
    limitSwitchPressed = false; // Reset limit switch press state
  }

  // Update stopwatch time
  if (running) {
    unsigned long currentMillis = millis();
    elapsedMillis += currentMillis - previousMillis;
    previousMillis = currentMillis;
  } else {
    previousMillis = millis(); // Update previousMillis to the current time when stopped
  }

  // Display time on MAX7219
  displayTime(elapsedMillis);
}

void displayTime(unsigned long milliseconds) {
  // Calculate minutes, seconds, and hundredths of a second
  int hundredths = (milliseconds % 1000) / 10;
  int seconds = (milliseconds / 1000) % 60;
  int minutes = (milliseconds / (1000 * 60)) % 100;

  // Display minutes (2 digits)
  lc.setDigit(0, 7, minutes / 10, false);
  lc.setDigit(0, 6, minutes % 10, false);
  
  // Display colon
  lc.setChar(0, 5, ':', false);

  // Display seconds (2 digits)
  lc.setDigit(0, 4, seconds / 10, false);
  lc.setDigit(0, 3, seconds % 10, false);
  
  // Display colon
  lc.setChar(0, 2, ':', false);

  // Display hundredths of a second (2 digits)
  lc.setDigit(0, 1, hundredths / 10, false);
  lc.setDigit(0, 0, hundredths % 10, false);
}
