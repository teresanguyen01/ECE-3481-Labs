/*
The Fibonacci sequenceLinks to an external site. is defined by the recursion F0 = 0, F1 = 1, 
and Fn = Fn-1 + Fn-2 for n>1. A simple way to compute the nth Fibonacci number is the following 
C program (which also returns zero for negative arguments):

unsigned int fib(unsigned char n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fib(n-1)+fib(n-2);
}
Implement this recursive function using assembly language. 
Your implementation must use recursion (there are more efficient ways to implement 
the fib() function but you have to use recursion, since this is more about stack frames 
than implementing fib()). The result of the call to fib() should be displayed on the 
serial monitor. Your program should be able to work for any Fibonacci number that can 
be represented by an 8-bit value. Include your test cases in your submission as well. 
Use the template from part 1 for this part. For simplicity, you can simply ignore the 
second argument to the function. For the more adventurous, feel free to modify the "asm" 
directive to only use one argument.

*/

#include "testasm.h"

__attribute__((used)) // don't delete even if it isn't used
// unsigned char -- store small nonnegative int (8 bits - 1 byte)
// AVR-GCC: unsigned int = 16 bit
unsigned int testasm(unsigned char a)
{ 
   unsigned int out; 
   asm volatile (R"(
      ; set program counter to the address named start of assembly --> skip .data and .text
      ; label = name for an address in the instruction stream
      rjmp start_of_assembly
      ; assembler directives --> do not generate executable instructions themselves
      .data
      .text

start_of_assembly:
      mov r24, %1
      call fib
      rjmp end_of_assembly

fib: 
    tst r24
    breq fib_ret0

    cpi r24, 1
    breq fib_ret1

    push r24

    subi r24, 1
    call fib

    push r24
    push r25

    lds r18, 0
    pop r21
    pop r20
    pop r24

    push r20
    push r21

    subi r24, 2
    call fib

    pop r21
    pop r20

    add r24, r20
    adc r25, r21
    ret

fib_ret0: 
    clr r24
    clr r25
    ret

fib_ret1: 
    ldi r24, 1
    clr r25
    ret

end_of_assembly:
      ; copy two adjacent registers as a pair
      ; movw Rs, Rs so Rd <- Rs and Rd + 1 <- Rs + 1
      movw %0, r24

   )" : "=w" (out) // must live in a register pair for movw

      : "r" (a) // inputs in any register

      : "r24"); // this asm modifies r24 and r25 so don't expect any previous values in those registers

      return out; // 16 bit value
}