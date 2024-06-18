#include <TM1637Display.h>
#include <SPI.h>
#include <SD.h>

#define CLK 2    // CLK pin of the TM1637 connected to Arduino digital pin 2
#define DIO 3    // DIO pin of the TM1637 connected to Arduino digital pin 3
#define BUTTON_PIN 4 // Pin for the pushbutton
#define LED_PIN_LEFT 5  // Pin for the left LED
#define LED_PIN_RIGHT 6  // Pin for the right LED
#define SD_CS_PIN 10   // Pin for the SD card module CS

TM1637Display display(CLK, DIO);

unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isRunning = false;
bool isBroken = false;

File dataFile;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN_LEFT, OUTPUT);
  pinMode(LED_PIN_RIGHT, OUTPUT);
  display.setBrightness(0x0f); // set the brightness to maximum

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  // Create a new file on the SD card
  dataFile = SD.open("timings.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Timings:");
    dataFile.close();
  } else {
    Serial.println("Error opening timings.txt");
  }
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
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

        // Save the timing to the SD card
        saveTiming(elapsedTime);
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

      // Save the timing to the SD card
      saveTiming(elapsedTime);
    }
  }
  
  if (isRunning) {
    elapsedTime = millis() - startTime;
    displayTime(elapsedTime); // Display elapsed time in milliseconds and seconds
  } else {
    displayTime(elapsedTime); // Display elapsed time even when paused or broken
  }
  
  // Control the two additional LEDs
  if (isRunning) {
    digitalWrite(LED_PIN_LEFT, HIGH);
    digitalWrite(LED_PIN_RIGHT, HIGH);
  } else {
    digitalWrite(LED_PIN_LEFT, LOW);
    digitalWrite(LED_PIN_RIGHT, LOW);
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

void saveTiming(unsigned long milliseconds) {
  // Open the file in append mode
  dataFile = SD.open("timings.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Time: ");
    dataFile.print(milliseconds);
    dataFile.println(" milliseconds");
    dataFile.close();
    Serial.println("Timing saved.");
  } else {
    Serial.println("Error opening timings.txt");
  }
}
