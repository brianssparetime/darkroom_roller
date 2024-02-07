#pragma once
#include "TM1637.h"
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
    tm.point(1); // turns on the colon between digits
    #ifdef DEBUG
      Serial.println("zeroes()");
    #endif
}


void Display::update() {
}

void Display::display(uint16_t s) {
    displayTimeSeg(s);
}

void Display::dim() {
    tm.set(0);  //set brightness; 0-7(max)
}

void Display::separator(bool x) {
  tm.point(1 ? x : 0);
}


// display a value given in seconds as mm:ss on the segment display
void Display::displayTimeSeg(uint16_t seconds) {
    #ifdef DEBUG
      char buf[16];
      sprintf(buf, "disp %3d", seconds);
      Serial.println(buf);
    #endif
  uint8_t m = seconds / 60;
  uint8_t s = seconds % 60;

  tm.point(1);
  tm.display(3, s % 10);
  tm.display(2, s / 10 % 10);
  tm.display(1, m % 10);
  //tm.display(0, m / 10 % 10);
  uint8_t tens_mins = m / 10 % 10;
  if( tens_mins == 0) {
    tm.display(0, tens_mins); 
  } else {
    tm.clearDigit(0);
  }
}


void Display::displayDigit(uint8_t d, uint8_t v) {
  //assert(d <= 3);
  if(v > 9) {
    clearDigit(d);
  } else {
    tm.display(d,v);
  }
}

void Display::clearDigit(uint8_t d) {
  tm.clearDigit(d);
}