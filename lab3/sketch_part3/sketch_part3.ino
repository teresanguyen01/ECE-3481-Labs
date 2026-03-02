/*
For the I2C Version
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 disp(128, 64, &Wire);

const uint8_t OLED_ADDR = 0x3C;

const int ENC_A = 2;
const int ENC_B = 3;
const int ENC_SW = 4;

int encValue = 128;
int lastA = HIGH;
unsigned long lastStepMs = 0;
const unsigned long DEBOUNCE_MS = 2;

const int R = 4;
float x = 64, y = 32;
int dirX = 1;
int dirY = 1;

const float VMAX = 3.5;
const unsigned long FRAME_MS = 20;
unsigned long lastFrameMs = 0;

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

void readEncoder() {
  int a = digitalRead(ENC_A);
  int b = digitalRead(ENC_B);

  if (lastA == HIGH && a == LOW) {
    unsigned long now = millis();
    if (now - lastStepMs >= DEBOUNCE_MS) {
      lastStepMs = now;

      if (b == HIGH) encValue--;
      else           encValue++;

      encValue = clamp255(encValue);
    }
  }

  lastA = a;
}

void setup() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  Serial.begin(9600);

  if (!disp.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) {}
  }

  disp.clearDisplay();
  disp.display();
}

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
    } else if (x >= 127 - R) {
      x = 127 - R;
      dirX = -dirX;
    }

    if (y <= R) {
      y = R;
      dirY = -dirY;
    } else if (y >= 63 - R) {
      y = 63 - R;
      dirY = -dirY;
    }

    disp.clearDisplay();
    disp.fillCircle((int)(x + 0.5), (int)(y + 0.5), R, SSD1306_WHITE);

    disp.setTextSize(1);
    disp.setTextColor(SSD1306_WHITE);
    disp.setCursor(0, 0);
    disp.print("ENC:");
    disp.print(encValue);

    disp.display();
  }
}