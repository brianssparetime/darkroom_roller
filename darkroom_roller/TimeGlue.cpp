#pragma once
#include "Arduino.h"
#include "PinsGlobals.h"
#include "TimeGlue.h"
#include "Display.h"


void TimeGlue::displayInterval(uint16_t seconds) {
    #ifdef DEBUG
      char buf[16];
      sprintf(buf, "disp %3d", seconds);
      Serial.println(buf);
    #endif
  uint8_t m = seconds / 60;
  uint8_t s = seconds % 60;

  Display::separator(true);
  Display::setDigit(3, s % 10);
  Display::setDigit(2, s / 10 % 10);
  Display::setDigit(1, m % 10);
  Display::setDigit(0, m / 10 % 10);
  Display::displayAllDigits();
}

void TimeGlue::displayMSS(int8_t mss[]) {
  Display::separator(true);
  Display::setDigit(3, mss[2]);
  Display::setDigit(2, mss[1]);
  Display::setDigit(1, mss[0] % 10);
  Display::setDigit(0, mss[0] / 10 % 10);
  Display::displayAllDigits();
}


uint16_t TimeGlue::MSStoInterval(int8_t mss[]) {
  uint16_t retval = mss[0] + mss[1] * 10 + mss[2] * 60;
  return retval;
}
/*
// display a value given in seconds as mm:ss on the segment display
void TimeGlue::displayTime(uint8_t hours, uint8_t mins) {
  #ifdef DEBUG
      char buf[20];
      sprintf(buf, "disp %2d : %2d", hours, mins);
      Serial.println(buf);
    #endif

  tm.point(1);
  tm.display(3, mins % 10);  // 1 mins right
  tm.display(2, mins / 10); // 10 mins
  tm.display(1, hours % 10); // 1 hours
  if( hours >= 10) {
    tm.display(0, 1); // 10 hours left
  } else {
    tm.clearDigit(0);
  }
}
*/