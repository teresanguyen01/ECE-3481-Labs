#include "testasm.h"

void testAsm();

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("The sum of 3 and 4 is: ");
  Serial.println(testasm(3, 4));

  testAsm();
  pinMode (13, OUTPUT);
}

void testAsm() { 
  // test case 1
  for (int a = 0; a <= 255; a += 10)
  { 
    for (int b = 0; b <= 255; b += 15)
    { 
      unsigned int from_testasm = testasm(a, b);
      unsigned int expected = a + b; 
      if (from_testasm != expected)
      { 
        Serial.print("BAD: a + b = ");
        Serial.print(expected);
        Serial.print("Received: ");
        Serial.print(from_testasm);
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
