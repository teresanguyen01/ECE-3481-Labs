#include "concurrency.h"

#define RED_PIN 9
#define GREEN_PIN 10

void busy_delay(volatile unsigned long count) {
  while (count > 0) {
    count--;
  }
}

void p1(void) {
  pinMode(RED_PIN, OUTPUT);

  while (1) {
    digitalWrite(RED_PIN, HIGH);
    busy_delay(30000);
    digitalWrite(RED_PIN, LOW);
    busy_delay(30000);
  }
}

void p2(void) {
  pinMode(GREEN_PIN, OUTPUT);

  while (1) {
    digitalWrite(GREEN_PIN, HIGH);
    busy_delay(70000);
    digitalWrite(GREEN_PIN, LOW);
    busy_delay(70000);
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  if (process_create(p1, 64) < 0) {
    while (1) { }
  }

  if (process_create(p2, 64) < 0) {
    while (1) { }
  }
}

void loop() {
  process_start();
  while (1) { }
}