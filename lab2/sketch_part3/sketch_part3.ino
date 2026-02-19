/*

This code uses a push button to control the LED numbers. Each push flashes the next number.

Made by Lisa Chou and Teresa Nguyen

*/

#include <Arduino.h>

// D13
#define DATA_BIT  PB4
#define LATCH_BIT PB3   // D12
#define CLOCK_BIT PB5   // D11

int dataPin = 13;
int latchPin = 12;
int clockPin = 11;

#define BUTTON_PIN 2
#define DEBOUNCE_MS 200 // debounce milliseconds

volatile int currentDigit = 0;
volatile unsigned long lastInterruptTime = 0;

// 0 - 9 digit designs
byte digits[10][5] = {
  {0x3E,0x51,0x49,0x45,0x3E},
  {0x00,0x42,0x7F,0x40,0x00},
  {0x42,0x61,0x51,0x49,0x46},
  {0x21,0x41,0x45,0x4B,0x31},
  {0x18,0x14,0x12,0x7F,0x10},
  {0x27,0x45,0x45,0x45,0x39},
  {0x3C,0x4A,0x49,0x49,0x30},
  {0x01,0x71,0x09,0x05,0x03},
  {0x36,0x49,0x49,0x49,0x36},
  {0x06,0x49,0x49,0x29,0x1E}
};


void updateShiftRegister(byte rowData, byte col){
  PORTB &= ~(1 << PB4);

  shiftOut(dataPin, clockPin, MSBFIRST, col);
  shiftOut(dataPin, clockPin, MSBFIRST, rowData);

  PORTB |=  (1 << PB4);   // latch high
  PORTB &= ~(1 << PB4);   // latch l
}

// interrupt service route
void handleButtonInterrupt() {
  unsigned long now = millis();

  // check for debounces --> check within 100 ms
  if (now - lastInterruptTime > DEBOUNCE_MS) {
    currentDigit++; // increase the current digit
    if (currentDigit > 9) 
    { 
      currentDigit = 0;
    }
    lastInterruptTime = now; // update interruption time
  }
}

void setup() {

  // shift register pins
  DDRB |= (1 << DDB3) | (1 << DDB4) | (1 << DDB5);

  // button input with pull-up
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // attach interrupt on falling edge (button press)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

// display one digit code
void displayDigit(int num) {
  // just refresh the currently selected digit
  for (int row = 0; row < 5; row++) { // draw the digit once

    byte rowPattern = digits[currentDigit][row];

    // one column at a time (active LOW)
    updateShiftRegister(rowPattern, ~(8 << row)); // move the row 1 byte
    // turn on one col of the matrix and turn off the others
  }
}

// 3-4, 6-7, 8-9, 0-1 goes too fast 
void loop() {
    displayDigit(currentDigit);
}
