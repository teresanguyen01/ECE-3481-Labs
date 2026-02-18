#define LATCH_PIN  PB4  // Example: Pin 12 on Arduino
#define CLOCK_PIN  PB3  // Example: Pin 11 on Arduino
#define DATA_PIN   PB5  // Example: Pin 13 on Arduino

int dataPin = 13;
int latchPin = 12;
int clockPin = 11;


void setup() {
  // Configure I/O using DDRx registers
  DDRB |= (1 << DDB3) | (1 << DDB4) | (1 << DDB5);

}
// 5x7 Dot Matrix font for digits 0-9
byte digits[10][5] = {
  {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0
	{0x00, 0x42, 0x7F, 0x40, 0x00},// 1
	{0x42, 0x61, 0x51, 0x49, 0x46},// 2
	{0x21, 0x41, 0x45, 0x4B, 0x31},// 3
	{0x18, 0x14, 0x12, 0x7F, 0x10},// 4
	{0x27, 0x45, 0x45, 0x45, 0x39},// 5
	{0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
	{0x01, 0x71, 0x09, 0x05, 0x03},// 7
	{0x36, 0x49, 0x49, 0x49, 0x36},// 8
	{0x06, 0x49, 0x49, 0x29, 0x1E}// 9
};
void updateShiftRegister(byte rowData, byte col){
  PORTB &= ~(1 << PB4);

  shiftOut(dataPin, clockPin, MSBFIRST, col);
  shiftOut(dataPin, clockPin, MSBFIRST, rowData);

  PORTB |=  (1 << PB4);   // latch high
  PORTB &= ~(1 << PB4);   // latch l
}


void loop() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 40; j++) {
      for (int row = 0; row < 5; row++) {
        byte rowPattern = digits[i][row]; // Get the row pattern for the digit
        updateShiftRegister(rowPattern, ~(8 << row));  // Send the row data
        delay(2);
      }
    
  }
  }
}
