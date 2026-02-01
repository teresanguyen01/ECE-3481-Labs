# Lab 1: Assembly Language Programming

## Lab Partners: Teresa Nguyen and Lisa Chou

# sketch_part1

`sketch_part1` contains the CPP code that adds two unsigned 8 bit integers a and b and the result is a 16-but value. The numbers contained in a and b are contained in r24 and r25.

The numbers are then added using `add r24, r25` and we `clr r25` for the carry operation and ``adc r`25, r1`` to add the carry to the higher byte.

The test cases are made using two for loops for different values of a and b and the results are compared with the expected results. The results after running should look like the following:

```
The sum of 3 and 4 is: 7
Passed all tests!
```

# sketch_part2

`sketch_part2` contains the CPP code that has two functions in assembly: `sumval` and `diffval`. We first move the values of a and b into registers r24 and r25 respectively and then we call `sumval` to get the sum of r24 and r25, clr r24, adc r25, r1 to get the carry and store the result in r20. This code was used during our `sketch_part1` implementation.

We then called `diffval` to get the difference of a and b. We use `cp r25, r24` to compare the two values. If a >= b, we jump to line 80 and subtract b from a using `sub r24, r25` and clr r25 to clear the higher byte. If a < b, we jump to line 90 and do `sub r25, r24`, move r25 to r24 and clr r25 to clear the higher byte. We then `add r24, r20` and `adc r25, r21` to add the carry to the higher byte.

The tests are around the same as in `sketch_part1` with two for loops for different values of a and b and the results are compared with the expected results. We made our own function that implemented the `sumval(a, b) + diffval(a, b)` to compare it to the assembly. The results after running should look like the following:

```
Passed all tests!
```

# sketch_part3

`sketch_part3` contains the CPP code that implements the Fibonacci sequence in assembly. We use recursion. Since we only required one argument, we stored the value of our argument in `r24` and changed the `.h` header to account for this. We first check if n is 0, and `breq fib_ret0` to return 0. We do `cpi r24, 1` and `breq fib_ret1` to return 1 if n is 1. Otherwise, to handle the recursion of an n >= 2, we save the original n on the stack by `push r24`, and then we need to calculate `fib(n-1)`. We do this by `subi r24, 1` to subtract by 1 and then call `fib` for the recursion, which returns `fib(n-1)` . We then push the result by `push r24` and `push r25` and then we pop fib(n-1) high and low into a temporary and pop the saved n into r24, and then push fib(n-1) for later availabiliy. We then push r20 and r21 to save and then calculate `fib(n-2)` by `subi r24, 2` and calling `fib` again. After, we pop r21 and r20 to get fib(n-1) and add the two results together using `add r24, r20` and `adc r25, r21`. Finally, we return from the function.

The test cases are similar, except we had to increase the increment since the fibonacci numbers have high time complexity without dynamic programming. The results after running should look like the following:

```
Passed fib(0)
Passed fib(10)
Passed fib(20)
Passed fib(30)
Passed all tests!
```
