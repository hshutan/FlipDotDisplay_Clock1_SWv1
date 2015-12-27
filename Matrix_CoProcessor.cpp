/*
 * This module will contain useful functions for driving
 * one of the two 45x7 panels inside a Luminator MAX3000 Side Sign.
 *
 * Harrison Shutan - Dec 2015
 *
 * This module is designed to be used with my own 45x7 controller board.
 *
 * note that there is no error checking for matrix out of bounds coords.
 *
*/

#include "Arduino.h"
#include "Matrix_CoProcessor.h"

const byte font[][5] = {
  {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {},
  {0x00, 0x00, 0x00, 0x00, 0x00}, // 0x20 32
  {0x00, 0x00, 0x6f, 0x00, 0x00}, // ! 0x21 33
  {0x00, 0x07, 0x00, 0x07, 0x00}, // " 0x22 34
  {0x14, 0x7f, 0x14, 0x7f, 0x14}, // # 0x23 35
  {0x00, 0x07, 0x04, 0x1e, 0x00}, // $ 0x24 36
  {0x23, 0x13, 0x08, 0x64, 0x62}, // % 0x25 37
  {0x36, 0x49, 0x56, 0x20, 0x50}, // & 0x26 38
  {0x00, 0x00, 0x07, 0x00, 0x00}, // ' 0x27 39
  {0x00, 0x1c, 0x22, 0x41, 0x00}, // ( 0x28 40
  {0x00, 0x41, 0x22, 0x1c, 0x00}, // ) 0x29 41
  {0x14, 0x08, 0x3e, 0x08, 0x14}, // * 0x2a 42
  {0x08, 0x08, 0x3e, 0x08, 0x08}, // + 0x2b 43
  {0x00, 0x50, 0x30, 0x00, 0x00}, // , 0x2c 44
  {0x08, 0x08, 0x08, 0x08, 0x08}, // - 0x2d 45
  {0x00, 0x60, 0x60, 0x00, 0x00}, // . 0x2e 46
  {0x20, 0x10, 0x08, 0x04, 0x02}, // / 0x2f 47
  {0x3e, 0x51, 0x49, 0x45, 0x3e}, // 0 0x30 48
  {0x00, 0x42, 0x7f, 0x40, 0x00}, // 1 0x31 49
  {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 0x32 50
  {0x21, 0x41, 0x45, 0x4b, 0x31}, // 3 0x33 51
  {0x18, 0x14, 0x12, 0x7f, 0x10}, // 4 0x34 52
  {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 0x35 53
  {0x3c, 0x4a, 0x49, 0x49, 0x30}, // 6 0x36 54
  {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 0x37 55
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 0x38 56
  {0x06, 0x49, 0x49, 0x29, 0x1e}, // 9 0x39 57
  {0x00, 0x36, 0x36, 0x00, 0x00}, // : 0x3a 58
  {0x00, 0x56, 0x36, 0x00, 0x00}, // ; 0x3b 59
  {0x08, 0x14, 0x22, 0x41, 0x00}, // < 0x3c 60
  {0x14, 0x14, 0x14, 0x14, 0x14}, // = 0x3d 61
  {0x00, 0x41, 0x22, 0x14, 0x08}, // > 0x3e 62
  {0x02, 0x01, 0x51, 0x09, 0x06}, // ? 0x3f 63
  {0x3e, 0x41, 0x5d, 0x49, 0x4e}, // @ 0x40 64
  {0x7e, 0x09, 0x09, 0x09, 0x7e}, // A 0x41 65
  {0x7f, 0x49, 0x49, 0x49, 0x36}, // B 0x42 66
  {0x3e, 0x41, 0x41, 0x41, 0x22}, // C 0x43 67
  {0x7f, 0x41, 0x41, 0x41, 0x3e}, // D 0x44 68
  {0x7f, 0x49, 0x49, 0x49, 0x41}, // E 0x45 69
  {0x7f, 0x09, 0x09, 0x09, 0x01}, // F 0x46 70
  {0x3e, 0x41, 0x49, 0x49, 0x7a}, // G 0x47 71
  {0x7f, 0x08, 0x08, 0x08, 0x7f}, // H 0x48 72
  {0x00, 0x41, 0x7f, 0x41, 0x00}, // I 0x49 73
  {0x20, 0x40, 0x41, 0x3f, 0x01}, // J 0x4a 74
  {0x7f, 0x08, 0x14, 0x22, 0x41}, // K 0x4b 75
  {0x7f, 0x40, 0x40, 0x40, 0x40}, // L 0x4c 76
  {0x7f, 0x02, 0x0c, 0x02, 0x7f}, // M 0x4d 77
  {0x7f, 0x04, 0x08, 0x10, 0x7f}, // N 0x4e 78
  {0x3e, 0x41, 0x41, 0x41, 0x3e}, // O 0x4f 79
  {0x7f, 0x09, 0x09, 0x09, 0x06}, // P 0x50 80
  {0x3e, 0x41, 0x51, 0x21, 0x5e}, // Q 0x51 81
  {0x7f, 0x09, 0x19, 0x29, 0x46}, // R 0x52 82
  {0x46, 0x49, 0x49, 0x49, 0x31}, // S 0x53 83
  {0x01, 0x01, 0x7f, 0x01, 0x01}, // T 0x54 84
  {0x3f, 0x40, 0x40, 0x40, 0x3f}, // U 0x55 85
  {0x0f, 0x30, 0x40, 0x30, 0x0f}, // V 0x56 86
  {0x3f, 0x40, 0x30, 0x40, 0x3f}, // W 0x57 87
  {0x63, 0x14, 0x08, 0x14, 0x63}, // X 0x58 88
  {0x07, 0x08, 0x70, 0x08, 0x07}, // Y 0x59 89
  {0x61, 0x51, 0x49, 0x45, 0x43}, // Z 0x5a 90
  {0x3c, 0x4a, 0x49, 0x29, 0x1e}, // [ 0x5b 91
  {0x02, 0x04, 0x08, 0x10, 0x20}, // \ 0x5c 92
  {0x00, 0x41, 0x7f, 0x00, 0x00}, // ] 0x5d 93
  {0x04, 0x02, 0x01, 0x02, 0x04}, // ^ 0x5e 94
  {0x40, 0x40, 0x40, 0x40, 0x40}, // _ 0x5f 95
  {0x00, 0x00, 0x03, 0x04, 0x00}, // ` 0x60 96
  {0x20, 0x54, 0x54, 0x54, 0x78}, // a 0x61 97
  {0x7f, 0x48, 0x44, 0x44, 0x38}, // b 0x62 98
  {0x38, 0x44, 0x44, 0x44, 0x20}, // c 0x63 99
  {0x38, 0x44, 0x44, 0x48, 0x7f}, // d 0x64 100
  {0x38, 0x54, 0x54, 0x54, 0x18}, // e 0x65 101
  {0x08, 0x7e, 0x09, 0x01, 0x02}, // f 0x66 102
  {0x0c, 0x52, 0x52, 0x52, 0x3e}, // g 0x67 103
  {0x7f, 0x08, 0x04, 0x04, 0x78}, // h 0x68 104
  {0x00, 0x44, 0x7d, 0x40, 0x00}, // i 0x69 105
  {0x20, 0x40, 0x44, 0x3d, 0x00}, // j 0x6a 106
  {0x00, 0x7f, 0x10, 0x28, 0x44}, // k 0x6b 107
  {0x00, 0x41, 0x7f, 0x40, 0x00}, // l 0x6c 108
  {0x7c, 0x04, 0x18, 0x04, 0x78}, // m 0x6d 109
  {0x7c, 0x08, 0x04, 0x04, 0x78}, // n 0x6e 110
  {0x38, 0x44, 0x44, 0x44, 0x38}, // o 0x6f 111
  {0x7c, 0x14, 0x14, 0x14, 0x08}, // p 0x70 112
  {0x08, 0x14, 0x14, 0x18, 0x7c}, // q 0x71 113
  {0x7c, 0x08, 0x04, 0x04, 0x08}, // r 0x72 114
  {0x48, 0x54, 0x54, 0x54, 0x20}, // s 0x73 115
  {0x04, 0x3f, 0x44, 0x40, 0x20}, // t 0x74 116
  {0x3c, 0x40, 0x40, 0x20, 0x7c}, // u 0x75 117
  {0x1c, 0x20, 0x40, 0x20, 0x1c}, // v 0x76 118
  {0x3c, 0x40, 0x30, 0x40, 0x3c}, // w 0x77 119
  {0x44, 0x28, 0x10, 0x28, 0x44}, // x 0x78 120
  {0x0c, 0x50, 0x50, 0x50, 0x3c}, // y 0x79 121
  {0x44, 0x64, 0x54, 0x4c, 0x44}, // z 0x7a 122
  {0x00, 0x08, 0x36, 0x41, 0x41}, // { 0x7b 123
  {0x00, 0x00, 0x7f, 0x00, 0x00}, // | 0x7c 124
  {0x41, 0x41, 0x36, 0x08, 0x00}, // } 0x7d 125
  {0x04, 0x02, 0x04, 0x08, 0x04}, // ~ 0x7e 126
};


mcp::mcp(byte rckPin)
{
  // These items are ran when the class is instantiated.
  // I enabled SPI, setup the latching clock pin, then right away
  // put all FETs in their off states.
  SPI.setBitOrder(LSBFIRST);
  SPI.begin();
  pinMode(rckPin, OUTPUT);
  _rckPin = rckPin;
  HighImpedance();
}


// Turn a single dot off.
// Will only pulse a dot once. Pass in the x, y coords (starts at 1,1 for upper left)
// delayTime is the amount of MICROseconds to PULSE for
// recoverTime is the amount of MILLIseconds to pause after pulsing (for power supply voltage recovery)
void mcp::dotOff(byte x, byte y, unsigned int delayTime, unsigned int recoverTime) {

  byte SR1 = B01010101;
  byte SR2 = B01010101;
  byte SR3 = B01010101;
  byte SR4 = B01010101;
  byte SR5 = B01010101;
  byte SR6 = B01010101;
  byte SR7 = B01010101;
  byte SR8 = B01010101;
  byte SR9 = B01010101;
  byte SR10 = B01010101;
  byte SR11 = B01010101;
  byte SR12 = B01010101;
  byte SR13 = B01010101;

  // First determine the row.
  if (y < 5) {
    // Row is in SR1
    SR1 = Bit_Reverse(SR1);
    bitWrite(SR1, (y + y) - 1 - 1, 1);
    SR1 = Bit_Reverse(SR1);
  } else {
    // Row is in SR2
    SR2 = Bit_Reverse(SR2);
    bitWrite(SR2, (y + y) - 8 - 1 - 1, 1);
    SR2 = Bit_Reverse(SR2);
  }

  // Then determine the col.
  if (x < 2) {
    //SR2
    bitWrite(SR2, 0, 0); //dev
  } else if (x < 6) {
    //SR3
    SR3 = Bit_Reverse(SR3);
    bitWrite(SR3, (x + x) - 2 - 1, 0);
    SR3 = Bit_Reverse(SR3);
  } else if (x < 10) {
    //SR4
    SR4 = Bit_Reverse(SR4);
    bitWrite(SR4, ((x - 4) + (x - 4)) - 2 - 1, 0);
    SR4 = Bit_Reverse(SR4);
  } else if (x < 14) {
    //SR5
    SR5 = Bit_Reverse(SR5);
    bitWrite(SR5, ((x - 8) + (x - 8)) - 2 - 1, 0);
    SR5 = Bit_Reverse(SR5);
  } else if (x < 18) {
    //SR6
    SR6 = Bit_Reverse(SR6);
    bitWrite(SR6, ((x - 12) + (x - 12)) - 2 - 1, 0);
    SR6 = Bit_Reverse(SR6);
  } else if (x < 22) {
    //SR7
    SR7 = Bit_Reverse(SR7);
    bitWrite(SR7, ((x - 16) + (x - 16)) - 2 - 1, 0);
    SR7 = Bit_Reverse(SR7);
  } else if (x < 26) {
    //SR8
    SR8 = Bit_Reverse(SR8);
    bitWrite(SR8, ((x - 20) + (x - 20)) - 2 - 1, 0);
    SR8 = Bit_Reverse(SR8);
  } else if (x < 30) {
    //SR9
    SR9 = Bit_Reverse(SR9);
    bitWrite(SR9, ((x - 24) + (x - 24)) - 2 - 1, 0);
    SR9 = Bit_Reverse(SR9);
  } else if (x < 34) {
    //SR10
    SR10 = Bit_Reverse(SR10);
    bitWrite(SR10, ((x - 28) + (x - 28)) - 2 - 1, 0);
    SR10 = Bit_Reverse(SR10);
  } else if (x < 38) {
    //SR11
    SR11 = Bit_Reverse(SR11);
    bitWrite(SR11, ((x - 32) + (x - 32)) - 2 - 1, 0);
    SR11 = Bit_Reverse(SR11);
  } else if (x < 42) {
    //SR12
    SR12 = Bit_Reverse(SR12);
    bitWrite(SR12, ((x - 36) + (x - 36)) - 2 - 1, 0);
    SR12 = Bit_Reverse(SR12);
  } else if (x < 46) {
    //SR13
    SR13 = Bit_Reverse(SR13);
    bitWrite(SR13, ((x - 40) + (x - 40)) - 2 - 1, 0);
    SR13 = Bit_Reverse(SR13);
  }

  // Then write to display.
  // Starts at SR13 and counts down because we want to send
  // the stream of data out LSB for the whole string of shift registers.
  SPI.transfer(SR13); //SR13
  SPI.transfer(SR12); //SR12
  SPI.transfer(SR11); //SR11
  SPI.transfer(SR10); //SR10
  SPI.transfer(SR9); //SR9
  SPI.transfer(SR8); //SR8
  SPI.transfer(SR7); //SR7
  SPI.transfer(SR6); //SR6
  SPI.transfer(SR5); //SR5
  SPI.transfer(SR4); //SR4
  SPI.transfer(SR3); //SR3
  SPI.transfer(SR2); //SR2
  SPI.transfer(SR1); //SR1
  digitalWrite (_rckPin, LOW);
  digitalWrite (_rckPin,  HIGH);
  delayMicroseconds(delayTime);
  HighImpedance();
  delay(recoverTime);
}


// Turn a single dot on.
// Will only pulse a dot once. Pass in the x, y coords (starts at 1,1 for upper left)
// delayTime is the amount of MICROseconds to PULSE for
// recoverTime is the amount of MILLIseconds to pause after pulsing (for power supply voltage recovery)
void mcp::dotOn(byte x, byte y, unsigned int delayTime, unsigned int recoverTime) {

  byte SR1 = B01010101;
  byte SR2 = B01010101;
  byte SR3 = B01010101;
  byte SR4 = B01010101;
  byte SR5 = B01010101;
  byte SR6 = B01010101;
  byte SR7 = B01010101;
  byte SR8 = B01010101;
  byte SR9 = B01010101;
  byte SR10 = B01010101;
  byte SR11 = B01010101;
  byte SR12 = B01010101;
  byte SR13 = B01010101;
  // First determine the row.
  if (y < 5) {
    // Row is in SR1
    SR1 = Bit_Reverse(SR1);
    bitWrite(SR1, (y + y) - 1, 0);
    SR1 = Bit_Reverse(SR1);
  } else {
    // Row is in SR2
    SR2 = Bit_Reverse(SR2);
    bitWrite(SR2, (y + y) - 8 - 1, 0);
    SR2 = Bit_Reverse(SR2);
  }

  // Then determine the col.
  if (x < 2) {
    //SR2
    bitWrite(SR2, 1, 1); //dev
  } else if (x < 6) {
    //SR3
    SR3 = Bit_Reverse(SR3);
    bitWrite(SR3, (x + x) - 2 - 1 - 1, 1);
    SR3 = Bit_Reverse(SR3);
  } else if (x < 10) {
    //SR4
    SR4 = Bit_Reverse(SR4);
    bitWrite(SR4, ((x - 4) + (x - 4)) - 2 - 1 - 1, 1);
    SR4 = Bit_Reverse(SR4);
  } else if (x < 14) {
    //SR5
    SR5 = Bit_Reverse(SR5);
    bitWrite(SR5, ((x - 8) + (x - 8)) - 2 - 1 - 1, 1);
    SR5 = Bit_Reverse(SR5);
  } else if (x < 18) {
    //SR6
    SR6 = Bit_Reverse(SR6);
    bitWrite(SR6, ((x - 12) + (x - 12)) - 2 - 1 - 1, 1);
    SR6 = Bit_Reverse(SR6);
  } else if (x < 22) {
    //SR7
    SR7 = Bit_Reverse(SR7);
    bitWrite(SR7, ((x - 16) + (x - 16)) - 2 - 1 - 1, 1);
    SR7 = Bit_Reverse(SR7);
  } else if (x < 26) {
    //SR8
    SR8 = Bit_Reverse(SR8);
    bitWrite(SR8, ((x - 20) + (x - 20)) - 2 - 1 - 1, 1);
    SR8 = Bit_Reverse(SR8);
  } else if (x < 30) {
    //SR9
    SR9 = Bit_Reverse(SR9);
    bitWrite(SR9, ((x - 24) + (x - 24)) - 2 - 1 - 1, 1);
    SR9 = Bit_Reverse(SR9);
  } else if (x < 34) {
    //SR10
    SR10 = Bit_Reverse(SR10);
    bitWrite(SR10, ((x - 28) + (x - 28)) - 2 - 1 - 1, 1);
    SR10 = Bit_Reverse(SR10);
  } else if (x < 38) {
    //SR11
    SR11 = Bit_Reverse(SR11);
    bitWrite(SR11, ((x - 32) + (x - 32)) - 2 - 1 - 1, 1);
    SR11 = Bit_Reverse(SR11);
  } else if (x < 42) {
    //SR12
    SR12 = Bit_Reverse(SR12);
    bitWrite(SR12, ((x - 36) + (x - 36)) - 2 - 1 - 1, 1);
    SR12 = Bit_Reverse(SR12);
  } else if (x < 46) {
    //SR13
    SR13 = Bit_Reverse(SR13);
    bitWrite(SR13, ((x - 40) + (x - 40)) - 2 - 1 - 1, 1);
    SR13 = Bit_Reverse(SR13);
  }

  // Then write to display.
  SPI.transfer(SR13); //SR13
  SPI.transfer(SR12); //SR12
  SPI.transfer(SR11); //SR11
  SPI.transfer(SR10); //SR10
  SPI.transfer(SR9); //SR9
  SPI.transfer(SR8); //SR8
  SPI.transfer(SR7); //SR7
  SPI.transfer(SR6); //SR6
  SPI.transfer(SR5); //SR5
  SPI.transfer(SR4); //SR4
  SPI.transfer(SR3); //SR3
  SPI.transfer(SR2); //SR2
  SPI.transfer(SR1); //SR1
  digitalWrite (_rckPin, LOW);
  digitalWrite (_rckPin,  HIGH);
  delayMicroseconds(delayTime);
  HighImpedance();
  delay(recoverTime);
}


// High Impedance is the "idle" state of the FETs. This means
// the gates of the N FETs are pulled low, and the gates of
// the P FETs are pulled high. Turning all FETs off.
void mcp::HighImpedance()
{
  for (int i = 0; i < 13; i++) {
    SPI.transfer(B01010101);
  }
  digitalWrite (_rckPin, LOW);
  digitalWrite (_rckPin,  HIGH);
}

void mcp::drawChar(byte x, byte y, char c, unsigned int onDelayTime, unsigned int offDelayTime, unsigned int onRecoverTime, unsigned int offRecoverTime, bool drawBlack, byte numOnPulses, byte numOffPulses)
{
  // The font array is 0 indexed, and my display is 1 indexed, ugh.
  for (int xx = 0; xx < 5; xx++) { // Loop through the columns of the font
    for (int yy = 0; yy < 7; yy++) { // Loop through the rows of the font

      if (bitRead(font[c][xx], yy)) { // Read a single bit of the applicable location within the two above for loops
        // If the bit == 1, then turn the dot on
        for (int i = 0; i < (numOnPulses + 1); i++) { // i is how many times to draw the letter.
          dotOn(x + xx, y + yy, onDelayTime, onRecoverTime);
        }

      } else {
        // Else, the bit == 0, so turn the dot off (if user wanted that)
        if (drawBlack) {
          for (int i = 0; i < (numOffPulses + 1); i++) { // i is how many times to draw the letter.
            dotOff(x + xx, y + yy, offDelayTime, offRecoverTime);
          }
        }
      }

    }
  }
}

void mcp::drawCharInverse(byte x, byte y, char c, unsigned int onDelayTime, unsigned int offDelayTime, unsigned int onRecoverTime, unsigned int offRecoverTime, bool drawBlack, byte numOnPulses, byte numOffPulses)
{
  // The font array is 0 indexed, and my display is 1 indexed, ugh.
  for (int xx = 0; xx < 5; xx++) { // Loop through the columns of the font
    for (int yy = 0; yy < 7; yy++) { // Loop through the rows of the font

      if (bitRead(font[c][xx], yy)) { // Read a single bit of the applicable location within the two above for loops
        // If the bit == 1, then turn the dot on
        for (int i = 0; i < (numOffPulses + 1); i++) { // i is how many times to draw the letter.
          dotOff(x + xx, y + yy, offDelayTime, offRecoverTime);
        }
      } else {
        // Else, the bit == 0, so turn the dot off (if user wanted that)
        if (drawBlack) {
          for (int i = 0; i < (numOnPulses + 1); i++) { // i is how many times to draw the letter.
            dotOn(x + xx, y + yy, onDelayTime, onRecoverTime);
          }
        }
      }

    }
  }
}

// Reverse the order of bits in a byte.
// I.e. MSB is swapped with LSB, etc.
// This is from the internet. I noticed no speed issues
// even compared to a bit reversing lookup table.
// There would have been faster ways to use bitWrite than the way I am without
// doing this, but it ended up working just fine with no performance issues.
byte mcp::Bit_Reverse(byte x)
{
  //          01010101  |         10101010
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  //          00110011  |         11001100
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  //          00001111  |         11110000
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}
