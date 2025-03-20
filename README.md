This ie example of LCD 1602A screen working with Raspberry pi in 4 bits mode (with less cables). The program is written in C and uses WiringPi lib when it is compiled.

You need to check your pins and update them accordingly

##Functions:
lcd_send_nibble(): Sends 4 bits at a time.
lcd_send_byte(): Sends a full 8-bit command/data in two 4-bit cycles.
lcd_init(): Initializes LCD in 4-bit mode.
lcd_set_cursor(): Moves cursor to a specific position.
lcd_print(): Prints a string to the LCD.

Since the LCD is operating in 4-bit mode, all data must be correctly transmitted over D4-D7. Double-check:
    D4, D5, D6, D7 pins are correctly connected to the Raspberry Pi.
    RS, E, RW (should be GND) are properly wired.
    Power (VCC to 5V, GND to GND) is secure.

##Compilation & Execution:

```Bash
  gcc lcd1602.c -o lcd1602 -lwiringPi
  sudo ./lcd1602
