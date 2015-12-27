
/*
 * 45x7 Flip Dot Matrix Clock
 * Clock version/type: 1
 * 
 * HLS 2015-12-24
 *
 * Dev'd on Arduino Pro Mini 8MHz (Like UNO)
 * Runs on (Rev 1) of my 45x7 controller board.
 * Planned for Teensy 3.2
 * 
 * UNO's hardware SPI pins:
 * 11, MOSI, SER on my board
 * 12, MISO, Not used for me.
 * 13, SCK (Shift Reg Clock)
 * Any GPIO:
 * 10, Shift Reg Latching Clock, RCK
 */

#include <SPI.h> // Uses default SPI pins.
#include "Matrix_CoProcessor.h" // My 45x7 controller helper class.
#include <Time.h> // PJRC's Arduino time class.
#include <Wire.h>  // Uses default SDA/SCL pins.
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
//#include <Metro.h> // A handy timer-job utility class.

//Metro metroWriteDisplay = Metro(500);         // ms

unsigned int onPulseLengthMicros = 200;  // 1000 = 1ms
unsigned int offPulseLengthMicros = 250; // 1000 = 1ms
unsigned int onRecoverTime = 20;
unsigned int offRecoverTime = 15;
// 15 works great if you do a second pass of writes to the display.
// Scoping shows that, at 15v input thru 60mA current limitor, [full]recovery time is around 60 milliseconds.
// So the closer to 60 this is set, the closer to a full force pulse. BUT it starts affecting nearby dots.


time_t lastTime = 0;

mcp mcp(10); // Create a new Matrix CoProcessor and tell it that pin 10 is the latching clock pin.

void setup()
{
  //setTime(23, 58, 35, 22, 12, 2015); // Useful for testing specific time cases.
  setSyncProvider(RTC.get); // Set time from RTC on boot up.
  setSyncInterval(3600); // Re-read time from RTC every X seconds.

//  Initial clock setting routine ---- setTime(hours, minutes, seconds, days, months, years);
//  setTime(12, 7, 39, 25, 12, 2015);
//  RTC.set(now());


  lastTime = now();

  fullDisplayClear();

  FreshWriteHour(lastTime);
  FreshWriteColon();
  FreshWriteMinute(lastTime);
  FreshWriteAMPM(lastTime);
}

void loop()
{
  if (minute(lastTime) != minute(now())) {
    printTimeStyle1(lastTime, now());
  }

  delay(1); // Slow down the loop.
}

time_t printTimeStyle1(time_t _lastTime, time_t _newTime) {

  if (hour(_lastTime) != hour(_newTime)) {
    // CleverEraseHour(_lastTime);
    // FreshWriteHour(_newTime);

    fullDisplayClear();

    FreshWriteHour(_newTime);
    FreshWriteColon();
    FreshWriteMinute(_newTime);
    FreshWriteAMPM(_newTime);
    FreshWriteHour(_newTime);
    FreshWriteMinute(_newTime);

  } else {

    if (minute(_lastTime) != minute(_newTime)) {
      CleverEraseMinute(_lastTime);
      FreshWriteMinute(_newTime);
    }

    //    if (isAM(_lastTime) != isAM(_newTime)) {
    //      CleverEraseAMPM(_lastTime);
    //      FreshWriteAMPM(_newTime);
    //    }

    FreshWriteHour(_newTime);
    FreshWriteMinute(_newTime);
    //    FreshWriteAMPM(_newTime);
//    FreshWriteHour(_newTime);
//    FreshWriteMinute(_newTime);
    //   FreshWriteAMPM(_newTime);
  }

  lastTime = _newTime;
}



void FreshWriteHour(time_t inTime) {
  byte _hour = hour(inTime);

  if (_hour > 12) {
    _hour = _hour - 12;
  }

  if (_hour == 0) {
    _hour = 12;
    mcp.drawChar(4, 1, String(_hour)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawChar(10, 1, String(_hour)[1], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else if (_hour < 10) {
    mcp.drawChar(10, 1, String(_hour)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else if (_hour > 9) {
    mcp.drawChar(4, 1, String(_hour)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawChar(10, 1, String(_hour)[1], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  }
}



void FreshWriteColon() {
  mcp.drawChar(16, 1, ':', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
}

void FreshWriteMinute(time_t inTime) {

  byte minVar = minute(inTime);
  if (minVar > 9) {
    mcp.drawChar(21, 1, String(minVar)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawChar(27, 1, String(minVar)[1], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else {
    mcp.drawChar(21, 1, '0', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawChar(27, 1, String(minVar)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  }
}

void FreshWriteAMPM(time_t inTime) {
  if (isAM(inTime)) {
    mcp.drawChar(37, 1, 'a', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else {
    mcp.drawChar(37, 1, 'p', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  }

}

void CleverEraseHour(time_t inTime) {
  byte _hour = hour(inTime);

  if (_hour > 12) {
    _hour = _hour - 12;
  }

  if (_hour == 0) {
    _hour = 12;
    mcp.drawCharInverse(4, 1, String(_hour)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawCharInverse(10, 1, String(_hour)[1], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else if (_hour < 10) {
    mcp.drawCharInverse(10, 1, String(_hour)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else if (_hour > 9) {
    mcp.drawCharInverse(4, 1, String(_hour)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawCharInverse(10, 1, String(_hour)[1], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  }
}


void CleverEraseMinute(time_t inTime) {

  byte minVar = minute(inTime);
  if (minVar > 9) {
    mcp.drawCharInverse(21, 1, String(minVar)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawCharInverse(27, 1, String(minVar)[1], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else {
    mcp.drawCharInverse(21, 1, '0', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
    mcp.drawCharInverse(27, 1, String(minVar)[0], onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  }
}

void CleverEraseAMPM(time_t inTime) {
  if (isAM(inTime)) {
    mcp.drawCharInverse(37, 1, 'a', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  } else {
    mcp.drawCharInverse(37, 1, 'p', onPulseLengthMicros, offPulseLengthMicros, onRecoverTime, offRecoverTime, false, 1, 1);
  }

}




void fullDisplayClear() {
  for (int x = 1; x < 45; x++) {
    for (int y = 1; y < 8; y++) {
      mcp.dotOff(x, y, offPulseLengthMicros + 100, offRecoverTime + 10);
      mcp.dotOff(x, y, offPulseLengthMicros + 100, offRecoverTime + 10);
    }
  }

  //fix for weird backwards dot on my display.
  mcp.dotOn(45, 7, offPulseLengthMicros, offRecoverTime);
  //fix for weird backwards dot on my display.
  mcp.dotOn(45, 7, offPulseLengthMicros, offRecoverTime);
}
