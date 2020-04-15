# TM1638NR
A tiny TM1638 LED &amp; key module library for Wiring/Arduino.  The buttons example uses only 182 bytes of flash using <a href="https://github.com/nerdralph/picoCore">picoCore</a> for the ATtiny13.

This library supports TM1638-based modules with 8 LEDs, an 8-digit 7-segment display, and 8 pushbuttons.
<img src="https://2.bp.blogspot.com/-NZFOGi6Pwxo/WxQZdbg0gCI/AAAAAAAApmY/oVZqeb0qvZYuGb-WmVdysZR6LpqqB-AIgCLcBGAs/s1600/LEDButtonModule.jpg">

#Usage
To use the library, STROBE, CLOCK and DATA must be set <a href="http://nerdralph.blogspot.com/2018/06/writing-small-and-simple-arduino-code.html">as decribed in my blog post</a>.  In setup() call TM1638NR::reset(), which takes an optional integer between 1 and 7 for the display brightness.

TM1638NR::readButtons returns a byte where each bit represents 1 button.  The LSB is set for the rightmost button (S8), and the MSB is set for the leftmost button (S1).

