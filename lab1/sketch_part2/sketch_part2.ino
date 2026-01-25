#include "testasm.h"

void testAsm();
void test_sum_diff(); 

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);

  test_sum_diff();
  pinMode (13, OUTPUT);
}

void test_sum_diff() { 
  for (int a = 0; a <= 255; a += 10)
  { 
    for (int b = 0; b <= 255; b += 15)
    { 
      unsigned int from_testasm = testasm((unsigned char)a, (unsigned char)b);
      unsigned int expected = (unsigned int)a + (unsigned int)b + (unsigned int)abs((int)a - (int)b);
      if (from_testasm != expected)
      { 
        Serial.print("\n");
        Serial.println("TEST CASE -------");
        Serial.print("a = "); 
        Serial.println(a); 
        Serial.print("b = ");
        Serial.println(b);
        Serial.print("BAD: a + b = ");
        Serial.println(expected);
        Serial.print("Received: ");
        Serial.println(from_testasm);
        return;
      }
    }
  }
  Serial.println("Passed all tests!");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
