Tasks:

1. look at AVR instruction set
2. programming environment of Ardunio
3. calling conventions

Goal: write assembly for AVR microcontroller

1. construct (built into C++ compiler) --> mix assembly and C++ using _asm_ keyword --> _asm volatile_ and volatile tells the compiler not to touch the assembly language sequence that has been specified (no optimizations!)
   `sketch_asm` ino contains the driver --> output values in serial monitor, continuously blinks the built-in LED on UNO
   `testasm.cpp` contains the assembly language
   Write an assembly language program that adds two numbers. The 2 numbers to be added are the unsigned 8-bit integers "a" and "b" (testasm function) and the result is a 16-bit value corresponding to their sum.
   Save and restore any register being modified (see testasm function and you will need to use the `carry` flag)

Notes:

- edit the cpp file. ino is the driver. testasm
