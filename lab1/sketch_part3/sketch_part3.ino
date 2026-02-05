#include "testasm.h"

void testAsm();
void test_fib(); 

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);

  test_fib();
  pinMode (13, OUTPUT);
}

void test_fib() { 
for (int a = 0; a <= 24; a++)
  { 
    unsigned int from_testasm = testasm((unsigned char)a);
    unsigned int expected = fibonacci(a); 
    if (from_testasm != expected)
    { 
        Serial.print("\n");
        Serial.println("TEST CASE -------");
        Serial.print("a = "); 
        Serial.println(a); 
        Serial.print("BAD: fib(a) = ");
        Serial.println(expected);
        Serial.print("Received: ");
        Serial.println(from_testasm);
        return;
    }
    Serial.print("Passed fib("); 
    Serial.print(a); 
    Serial.println(")");
  }
  Serial.println("Passed all tests!");
}

int fibonacci(int n)
{ 
    if (n <= 1)
    { 
        return n;
    }
    else
    { 
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
