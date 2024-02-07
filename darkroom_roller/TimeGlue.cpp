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
  Display::displayAllDigits(true);
}

void TimeGlue::displayMSS(uint8_t mss[]) {
  Display::separator(true);
  Display::setDigit(3, mss[2]);
  Display::setDigit(2, mss[1]);
  Display::setDigit(1, mss[0] % 10);
  Display::setDigit(0, mss[0] / 10 % 10);
  Display::displayAllDigits(false);
}


uint16_t TimeGlue::MSStoInterval(uint8_t mss[]) {
  uint16_t retval = mss[0] + mss[1] * 10 + mss[2] * 60;
  return retval;
}