# Lab 2: Digital I/O and Real-world Programming

## Lab Partners: Lisa Chou and Teresa Nguyen

### Part 1: Digital input by polling

This section contains ```sketch_part1.ino``` and the schematic ```sketch_part1_schematic.png``` for a push-button circuit with software debouncing. PD2 was configured as an input by clearing bit 2 of DDRD, and the internal pull-up was enabled by setting bit 2 of **PORTD**. The switch state was read directly from PIND, so no digitalRead() or pinMode() functions were used. Debouncing was implemented by tracking the last raw state, the last change time, and a stable state. A new state was accepted only after 20 ms, and the Serial Monitor printed **“PRESSED”** or **“RELEASED”** only when the debounced state changed.


### Part 2: Digital output: dot-matrix LED display

This section contains the code ```sketch_part2.ino``` and schematic ```sketch_part2_schematic.png``` for the LED display that cycles through flashing the numbers 0-9. We built functions such as to update the shift register, which loads the row and column data into 2 shift registers and latches it to the LED matrix to show one column. We also built the 5x7 dot matrix and configured out data, latch, and clock pins. Finally, we built loops to update the display.

### Part 3: Counter and interrupts

We combined the scripts from part 1 and part 2 to create part 3 (```sketch_part3.ino``` and ```sketch_part3_schematic.png```. The difference is that we created an interrupt service routine where we check for debouncing, increase the current digit/or wrap it, and update the last interruption time. We attached the interrupt to the setup and then created the loop and function (same code from part 2 except removed some for loops) to update the display.
