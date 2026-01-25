/*
This script implements the following two functions in assembly, respecting
all calling conventions.

unsigned int sumval(unsigned char a, unsigned char b)
{
    return a+b;
} 8 bit inputs --> 16 bit output

unsigned int diffval(unsigned char a, unsigned char b)
{
   if (a > b) {
      return a-b;
   }
   else {
      return b-a;
   } absolute difference 
}

Finally, the main program in assembly language should set the 
return registers (r25:r24, see the template) to the value 
"sumval(a,b)+diffval(a,b)", using function calls to invoke 
the two functions and combine their return values.

*/

#include "testasm.h"

__attribute__((used)) // don't delete even if it isn't used
// unsigned char -- store small nonnegative int (8 bits - 1 byte)
// AVR-GCC: unsigned int = 16 bit
unsigned int testasm(unsigned char a, unsigned char b)
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
      ; copy input a into r24 and copy input b into r25
      mov r24, %1
      mov r25, %2
      call sumval

      movw r20, r24

      mov r24, %1
      mov r25, %2

      call diffval

      add r24, r20
      adc r25, r21

      rjmp end_of_assembly

sumval: 
      add r24, r25
      ; set all 9 bits of r25 to 0 to set Z flag and clear C flag --> have to do to avoid the caller thinking the number is larger than
      ; it actually is
      clr r25
      ; 16 bit addition or propagate a carry
      adc r25, r1
      ret
   
diffval: 
      ; result needs to be in r24
      cp r24, r25  ; if a >= b --> C = 0, Z = 0 (Z is 0 and C is carry)
      brsh a_greater_than_equal_b
         sub r25, r24 ; subtracts r24 from r25
         mov r24, r25
         clr r25
         ret 

      a_greater_than_equal_b: 
         sub r24, r25 ; subtracts r25 from r24
         clr r25
         ret

end_of_assembly:
      ; copy two adjacent registers as a pair
      ; movw Rs, Rs so Rd <- Rs and Rd + 1 <- Rs + 1
      movw %0, r24

   )" : "=w" (out) // must live in a register pair for movw

      : "r" (a), "r" (b) // inputs in any register

      : "r25", "r24", "r21", "r20"); // this asm modifies r24 and r25 so don't expect any previous values in those registers

      return out; // 16 bit value
}