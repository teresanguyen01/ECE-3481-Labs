# Lab 3: Analog I/O and Digital I/O protocols
## Lisa Chou and Teresa Nguyen

### Part 1: Analog voltage input: photocell
We connected the photocell and a 10K resistor to form a voltage divider and read the voltage using analogRead() on A0. The ADC value ranges from 0–1023, so we converted it to 0–255 by dividing by 4. This value represents the light level, where 0 is dark and 255 is bright.

We printed the ADC value and the scaled PWM value to the Serial Monitor once every second using millis() to control the timing. The scaled value was also used with analogWrite() on the blue LED pin to control brightness using PWM. As the light on the photocell increased, the ADC value increased and the LED became brighter; covering the sensor decreased the value and dimmed the LED.

The video is located as ```lab3/sketch_part1/IMG_3546.mov```.

### Part 2: Time-encoded analog value: Ultrasonic range finder
We combined the photocell from Part 1 with the HC-SR04 ultrasonic sensor to control both brightness and color of the RGB LED. Once per second (using millis()), we read the photocell with analogRead(A0) and scaled it to a 0–255 brightness value by dividing by 4.

For distance, we triggered the HC-SR04 by sending a short pulse on TRIG, then measured the return pulse length on ECHO using pulseIn(). The echo time in microseconds was converted to centimeters using distanceCm = echoUs / 58.0, then rounded and clamped to a smaller, easier-to-test range of 10–40 cm. We mapped that distance to a 0–255 value where 0 is closest and 255 is farthest.

We used the mapped distance value to set the LED color as a gradient from green when close to red when far. Finally, we multiplied that base color by the photocell brightness so the distance controls the color and the photocell controls how bright the LED is overall. We printed brightness, distance, and the mapped 0–255 distance value to the Serial Monitor once per second to verify everything matched what we saw.

The video is located as ```lab3/sketch_part2/IMG_3547.mov```.

### Part 3: SPI/I2C display
We used the I2C OLED version by wiring SDA to A4 and SCL to A5, then initialized the display with the Adafruit SSD1306 library using address 0x3C. On startup we cleared the screen to make sure the display was working and ready to draw.

For the rotary encoder, we read ENC_A and ENC_B as digital inputs with pullups enabled. We tracked the encoder as a value from 0–255 starting at 128, and added a small debounce delay so it would not double count steps when turning quickly. One direction of rotation increases the value and the other decreases it, and we clamp it so it saturates at 0 and 255.

To draw the bouncing ball, we updated the screen every 20 ms. The ball has radius 4 and moves around the 128x64 display. The encoder value controls speed: 128 means stopped, values above or below 128 increase speed in opposite directions. We also treated the edges as walls, so when the ball hits a boundary it “bounces” by flipping its direction. We displayed the current encoder value on the screen so it was easy to see how turning the knob affected the motion.

The video is located as ```lab3/sketch_part3/My Movie.mov```.

