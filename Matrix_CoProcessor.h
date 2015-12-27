/*
 *
*/
#ifndef Matrix_CoProcessor_h
#define Matrix_CoProcessor_h

#include "Arduino.h"
#include <SPI.h>

class mcp
{
  public:
    mcp(byte rckPin);
    void HighImpedance();
    void dotOn(byte x, byte y, unsigned int delayTime, unsigned int recoverTime);
    void dotOff(byte x, byte y, unsigned int delayTime, unsigned int recoverTime);
    byte Bit_Reverse(byte x);
    void drawChar(byte x, byte y, char c, unsigned int onDelayTime, unsigned int offDelayTime, unsigned int onRecoverTime, unsigned int offRecoverTime, bool drawBlack, byte numOnPulses, byte numOffPulses);
    void drawCharInverse(byte x, byte y, char c, unsigned int onDelayTime, unsigned int offDelayTime, unsigned int onRecoverTime, unsigned int offRecoverTime, bool drawBlack, byte numOnPulses, byte numOffPulses);
  private:
    byte _rckPin;

};

#endif
