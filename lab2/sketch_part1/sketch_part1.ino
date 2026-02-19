// The goal of this part of the lab is to implement a polling-based approach to reading the state of a switch.

// Use the jumper wires, breadboard, and push-button switch to build a circuit that is connected to the PD2 pin on the ATmega328P.
// Use the PORTD, PIND, and DDRD I/O mapped registers to configure the pin as an input with a passive pull-up. 
// (Note: the I/O mapped registers are conveniently mapped to variables you can directly access.)

// Write code that displays the current state of the switch to the terminal, printing something only when the state of the switch changes.
// The program must include code that de-bounces the switch.

// For this part, do not use the Arduino digitalRead()/pinMode() functions. You can use these functions if 
// you want to debug your circuit on the breadboard, but the final implementation should not use these functions. 
// (Note: the implication here is that you are to read off-board values through the SFR registers that map to these port locations.)


/*

Using polling -- every time button is pushed, on the Serial Monitor it says PUSHED and
when I release the button, the terminal says RELEASE.

Made by Lisa Chou and Teresa Nguyen

*/

#include <Arduino.h>

const uint8_t BTN_MASK = (1 << 2);
const unsigned long DEBOUNCE_MS = 20;

bool stable = false;
bool rawPrev = false;
unsigned long tChanged = 0;

void setup() {
  Serial.begin(9600);

  DDRD &= ~BTN_MASK;
  PORTD |= BTN_MASK;

  // initialize
  rawPrev = ((PIND & BTN_MASK) == 0);
  stable = rawPrev;
  tChanged = millis();

  Serial.println("Ready.");
  Serial.println(stable ? "PRESSED" : "RELEASED");
}

void loop() {
  bool raw = ((PIND & BTN_MASK) == 0);
  unsigned long now = millis();

  if (raw != rawPrev) {
    rawPrev = raw;
    tChanged = now;
  }

  if ((now - tChanged) >= DEBOUNCE_MS && raw != stable) {
    stable = raw;
    Serial.println(stable ? "PRESSED" : "RELEASED");
  }
}
