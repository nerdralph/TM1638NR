#include <TM1638NR.h>

// change according to wiring
const byte TM1638NR::STROBE = 0;
const byte TM1638NR::CLOCK = 1;
const byte TM1638NR::DATA = 2;

void setup() {
  TM1638NR::reset();
}

void loop() {
  uint8_t buttons = TM1638NR::readButtons();
  TM1638NR::displayHex(0, buttons);

  // light LEDs when buttons are pressed 
  TM1638NR::setLEDs(buttons);
}
