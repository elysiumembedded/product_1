#include <LedControl.h>

// Pin Definitions
const int DIN_PIN = 11;
const int CS_PIN = 10;
const int CLK_PIN = 13;

// Create a LedControl object
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); // DIN, CLK, CS, number of displays

void setup() {
  // Initialize the MAX7219
  lc.shutdown(0, false);       // Wake up the MAX7219
  lc.setIntensity(0, 8);       // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);          // Clear display register
}

void loop() {
  // Display numbers from 0 to 9 on the 7-segment display
  for (int num = 0; num < 10; num++) {
    lc.setDigit(0, 0, num, false); // (display, digit, value, dp) - dp is decimal point
    delay(1000);                   // Wait for 1 second
  }
}
