/* Ralph Doncaster 2018
 * TM1638NR.h - control TM1638-based LED & key modules
 * MIT license.
 * 20200317 v1.2: new rxtx method
 * 20200318 v2.0 reverse bitorder for readButtons + size optimized
 * 20200326 v2.1 added displayHex -> displayNibble x 2
 * 20200415 v2.1.1 add comments & code cleanup
 */

#pragma once

/*
LED & Key module segments:
  -0-
 5   1
  -6-
 4   2
  -3- .7
*/

static const int8_t HEXSS[] PROGMEM = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* A */
  0x7C, /* B */
  0x39, /* C */
  0x5E, /* D */
  0x79, /* E */
  0x71, /* F */
};

class TM1638NR {
 public:
  // brightness levels 0-7 - see datasheet section 5.3
  static void reset(byte brightness = 2){
    send(DISPLAY_ON | (brightness & 0x07));
    // initialize display data (address 0x00 to 0x0F) to 0
    uint8_t i = 15;
    do {
      writeLoc(i, 0);
    } while (i--);
  }

  // 8 buttons from K3 supported
  // rightmost button (S8) = bit 0, leftmost (S1) = bit 7
  // see datasheet section VII for key scanning
  static uint8_t readButtons() {
    send(READ_KEYS);

    uint8_t buttons = 0;
    for (uint8_t i = 0; i < 4; i++) {
      buttons <<= 1;
      uint8_t scan = receive();           // read 2 buttons at a time
      buttons |= (scan >> 4 | scan << 4); // swap nibbles
    } 

    pinMode(STROBE, INPUT);
    return buttons;
  }

  // bit 0 = LED 0 ... 
  static void setLEDs(uint8_t mask) {
    // LEDs are at odd locations
    for ( uint8_t loc = 1; loc < 16; loc += 2) {
      writeLoc(loc, mask & 1);
      mask >>= 1;
    }
  }

  // write to seven segments - even locations
  static void displaySS(uint8_t position, uint8_t value) {
    writeLoc(position<<1, value);
  }

  // display lower nibble as hex (0-F)
  static void displayNibble(uint8_t position, uint8_t nibble) {
    displaySS(position, pgm_read_byte(HEXSS + (nibble & 0x0F)));
  }

  // display binary u8 as 2-digit hex number
  static void displayHex(uint8_t position, uint8_t value) {
    displayNibble(position, value / 16);
    displayNibble(position + 1, value);
  }

 private:
  // open-drain output, data clocked on rising edge, LSB first 1Mhz max
  static void send(uint8_t data) {
    rxtx(data);
  }

  // activate strobe and then receive/transmit
  static uint8_t rxtx(uint8_t data) {
    uint8_t bits = 8;
    pinMode(STROBE, OUTPUT);
    do {
      pinMode(CLOCK, OUTPUT);
      if (!(data & 0x01)) pinMode(DATA, OUTPUT);
      data /= 2;
      pinMode(CLOCK, INPUT);
      // wait for rise time
      while (digitalRead(CLOCK) == 0);
      if (digitalRead(DATA)) data |= 0x80;
      pinMode(DATA, INPUT);             // release data line
    }
    while (--bits);
    return data;
  }

  // data clocked from slave on falling edge, LSB first 1Mhz max
  static uint8_t receive() {
    return rxtx(0xFF);
  }

  // set address (0-F), then send data to write
  // see datasheet section 5.2 & X(2)
  static void writeLoc(uint8_t position, uint8_t value) {
    send(SET_ADDRESS | position);
    send(value);
    pinMode(STROBE, INPUT);
  }

  static const byte STROBE;
  static const byte CLOCK;
  static const byte DATA;

  enum COMMAND {
    READ_KEYS = 0x42,                   // datasheet s 5.1
    DISPLAY_ON = 0x88,                  // datasheet s 5.3
    SET_ADDRESS = 0xC0                  // datasheet s 5.2
  };

};

