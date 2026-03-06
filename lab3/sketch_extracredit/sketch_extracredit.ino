#include <Arduino.h>
#include <avr/io.h>
#include <string.h>
#include <math.h>

// Rotary encoder pins
const int ENC_A = 2;
const int ENC_B = 3;
const int ENC_SW = 4;

// OLED SPI pins
const int OLED_SI = 11;   // data
const int OLED_CLK = 13;  // clk
const int OLED_DC = 9;    // dc
const int OLED_RST = 8;   // rst
const int OLED_CS = 10;   // cs

// OLED constants
const int OLED_W = 128;
const int OLED_H = 64;

// 128 * 64 / 8 = 1024 bytes
uint8_t fb[OLED_W * OLED_H / 8];

// Encoder state
int encValue = 128;
int lastA = HIGH;
int lastPrinted = -1;
unsigned long lastStepMs = 0;
const unsigned long DEBOUNCE_MS = 2;

// Ball state
const int R = 4;
float x = 64, y = 32;
int dirX = 1;
int dirY = 1;

const float VMAX = 3.5;
const unsigned long FRAME_MS = 20;
unsigned long lastFrameMs = 0;

// Utility functions
int clamp255(int v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return v;
}

float encoderToSpeed(int v) {
  int centered = v - 128;
  if (centered == 0) return 0.0;

  float n = centered / 127.0;
  return n * VMAX;
}

// Rotary encoder
void readEncoder() {
  int a = digitalRead(ENC_A);
  int b = digitalRead(ENC_B);

  // detect falling edge on A
  if (lastA == HIGH && a == LOW) {
    unsigned long now = millis();
    if (now - lastStepMs >= DEBOUNCE_MS) {
      lastStepMs = now;

      if (b == HIGH) encValue--;
      else encValue++;

      encValue = clamp255(encValue);
    }
  }

  lastA = a;
}

// OLED control helpers
void csLow() {
  digitalWrite(OLED_CS, LOW);
}
void csHigh() {
  digitalWrite(OLED_CS, HIGH);
}

void dcCmd() {
  digitalWrite(OLED_DC, LOW);
}
void dcData() {
  digitalWrite(OLED_DC, HIGH);
}

// SPI using AVR registers
void spiInit() {
  // MOSI
  pinMode(OLED_SI, OUTPUT);  
  // SCK 
  pinMode(OLED_CLK, OUTPUT);  
  // SS must be output in master mode
  pinMode(OLED_CS, OUTPUT);   

  // Enable SPI, Master mode, Mode 0
  SPCR = _BV(SPE) | _BV(MSTR);

  // Double speed
  SPSR = _BV(SPI2X);
}

void spiWrite(uint8_t b) {
  SPDR = b;
  while (!(SPSR & _BV(SPIF))) {
  }
}

// SSD1306 write helpers
void oledWriteCmd(uint8_t c) {
  csLow();
  dcCmd();
  spiWrite(c);
  csHigh();
}

void oledWriteDataBurst(const uint8_t *data, int n) {
  csLow();
  dcData();
  for (int i = 0; i < n; i++) {
    spiWrite(data[i]);
  }
  csHigh();
}

// OLED reset + init
void oledReset() {
  digitalWrite(OLED_RST, LOW);
  delay(10);
  digitalWrite(OLED_RST, HIGH);
  delay(10);
}

void oledInit() {
  pinMode(OLED_DC, OUTPUT);
  pinMode(OLED_RST, OUTPUT);
  pinMode(OLED_CS, OUTPUT);

  csHigh();
  digitalWrite(OLED_RST, HIGH);

  spiInit();
  oledReset();

  // SSD1306 init sequence for 128x64 SPI display
  oledWriteCmd(0xAE);  // display off
  oledWriteCmd(0xD5);
  oledWriteCmd(0x80);  // clock divide
  oledWriteCmd(0xA8);
  oledWriteCmd(0x3F);  // multiplex ratio = 63
  oledWriteCmd(0xD3);
  oledWriteCmd(0x00);  // display offset = 0
  oledWriteCmd(0x40);  // start line = 0
  oledWriteCmd(0x8D);
  oledWriteCmd(0x14);  // charge pump on
  oledWriteCmd(0x20);
  oledWriteCmd(0x02);  // page addressing mode
  oledWriteCmd(0xA1);  // segment remap
  oledWriteCmd(0xC8);  // COM scan dec
  oledWriteCmd(0xDA);
  oledWriteCmd(0x12);  // COM pins for 128x64
  oledWriteCmd(0x81);
  oledWriteCmd(0x7F);  // contrast
  oledWriteCmd(0xD9);
  oledWriteCmd(0xF1);  // precharge
  oledWriteCmd(0xDB);
  oledWriteCmd(0x40);  // VCOM detect
  oledWriteCmd(0xA4);  // resume RAM display
  oledWriteCmd(0xA6);  // normal display
  oledWriteCmd(0xAF);  // display on
}

// =========================
// Framebuffer + drawing
// =========================
void oledClear() {
  memset(fb, 0, sizeof(fb));
}

void setPixel(int px, int py, bool on) {
  if (px < 0 || px >= OLED_W || py < 0 || py >= OLED_H) return;

  int index = (py / 8) * OLED_W + px;
  uint8_t mask = 1 << (py % 8);

  if (on) fb[index] |= mask;
  else fb[index] &= ~mask;
}

void fillCircle(int cx, int cy, int r, bool on) {
  for (int dy = -r; dy <= r; dy++) {
    for (int dx = -r; dx <= r; dx++) {
      if (dx * dx + dy * dy <= r * r) {
        setPixel(cx + dx, cy + dy, on);
      }
    }
  }
}

// very small rectangle helper for simple number display
void fillRect(int x0, int y0, int w, int h, bool on) {
  for (int y = y0; y < y0 + h; y++) {
    for (int x = x0; x < x0 + w; x++) {
      setPixel(x, y, on);
    }
  }
}

// simple 3x5 digit font for encValue
const uint8_t digitFont[10][5] = {
  { 0b111, 0b101, 0b101, 0b101, 0b111 },  // 0
  { 0b010, 0b110, 0b010, 0b010, 0b111 },  // 1
  { 0b111, 0b001, 0b111, 0b100, 0b111 },  // 2
  { 0b111, 0b001, 0b111, 0b001, 0b111 },  // 3
  { 0b101, 0b101, 0b111, 0b001, 0b001 },  // 4
  { 0b111, 0b100, 0b111, 0b001, 0b111 },  // 5
  { 0b111, 0b100, 0b111, 0b101, 0b111 },  // 6
  { 0b111, 0b001, 0b001, 0b001, 0b001 },  // 7
  { 0b111, 0b101, 0b111, 0b101, 0b111 },  // 8
  { 0b111, 0b101, 0b111, 0b001, 0b111 }   // 9
};

void drawDigit(int x0, int y0, int d) {
  if (d < 0 || d > 9) return;

  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 3; col++) {
      bool on = (digitFont[d][row] >> (2 - col)) & 1;
      if (on) setPixel(x0 + col, y0 + row, true);
    }
  }
}

void drawNumber(int x0, int y0, int value) {
  int hundreds = value / 100;
  int tens = (value / 10) % 10;
  int ones = value % 10;

  drawDigit(x0, y0, hundreds);
  drawDigit(x0 + 4, y0, tens);
  drawDigit(x0 + 8, y0, ones);
}

// Send framebuffer to OLED
void oledSetPageCol(uint8_t page, uint8_t col) {
  oledWriteCmd(0xB0 | page);                 // page address
  oledWriteCmd(0x00 | (col & 0x0F));         // lower col
  oledWriteCmd(0x10 | ((col >> 4) & 0x0F));  // higher col
}

void oledDisplay() {
  for (uint8_t page = 0; page < 8; page++) {
    oledSetPageCol(page, 0);
    oledWriteDataBurst(&fb[page * OLED_W], OLED_W);
  }
}

// Setup
void setup() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  Serial.begin(9600);

  oledInit();
  oledClear();
  oledDisplay();
}

// Main loop
void loop() {
  readEncoder();

  unsigned long now = millis();
  if (now - lastFrameMs >= FRAME_MS) {
    lastFrameMs = now;

    float speed = encoderToSpeed(encValue);

    x += dirX * speed;
    y += dirY * (speed * 0.7);

    // Bounce off edges
    if (x <= R) {
      x = R;
      dirX = -dirX;
    } else if (x >= (OLED_W - 1 - R)) {
      x = OLED_W - 1 - R;
      dirX = -dirX;
    }

    if (y <= R) {
      y = R;
      dirY = -dirY;
    } else if (y >= (OLED_H - 1 - R)) {
      y = OLED_H - 1 - R;
      dirY = -dirY;
    }

    // Draw frame
    oledClear();

    fillCircle((int)(x + 0.5), (int)(y + 0.5), R, true);

    // small box behind number for readability
    fillRect(0, 0, 13, 7, false);
    drawNumber(0, 1, encValue);

    oledDisplay();

    if (encValue != lastPrinted) {
  Serial.print("ENC: ");
  Serial.println(encValue);
  lastPrinted = encValue;
}
  }
}