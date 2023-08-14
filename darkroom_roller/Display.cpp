#pragma once
#include <TM1637.h>
#include "PinsGlobals.h"
#include "Display.h"
#include "Arduino.h"



TM1637 tm(DISP_CLK, DISP_DIO); // set up 47seg
TM1637* Display::_disp = &tm;





void Display::init() {
    tm.init();
    tm.set(1);  //set brightness; 0-7(max)
}

void Display::zeroes() {
    for(uint8_t i = 0; i < 4; i++) {
        tm.display(i,0);
    }
    tm.point(1); // what does this do?
}


void Display::update() {
}

void Display::display(uint16_t s) {
    displayTimeSeg(s);
}

void Display::setDot(uint8_t u) {
    // TODO IMPLEMENT!
}

void Display::dim() {
    tm.set(0);  //set brightness; 0-7(max)
}


// display a value given in seconds as mm:ss on the segment display
void Display::displayTimeSeg(uint16_t seconds) {
  uint8_t m = seconds / 60;
  uint8_t s = seconds % 60;

  tm.point(1);
  tm.display(3, s % 10);
  tm.display(2, s / 10 % 10);
  tm.display(1, m % 10);
  tm.display(0, m / 10 % 10);
}
