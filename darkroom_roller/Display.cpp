#pragma once
#include "TM1637.h"
#include "PinsGlobals.h"
#include "Display.h"
#include "Arduino.h"



TM1637 tm(DISP_CLK, DISP_DIO); // set up 47seg
TM1637* Display::_disp = &tm;

uint8_t Display::_digits[4] = {1,2,0,0};
uint8_t Display::_blink_digit = 1;



void Display::init() {
    tm.init();
    tm.set(2);  //set brightness; 0-7(max)
    _blink_digit = 1;
    separator(true);
}

void Display::zeroes() {
    for(uint8_t i = 0; i < 4; i++) {
        _digits[i] = 0;
    }
    #ifdef DEBUG
      Serial.println("zeroes()");
    #endif
}

void Display::separator(bool x) {
  tm.point(1 ? x : 0);
}

void Display::update() {

  // TODO:  consider whether setDigit should call this,
  // or whether it should just be done from the main loop


  for (uint8_t i = 0; i < 4; i++) {
    // if time to blink
    if (_blink_onoff && _blink_digit == i) {
      if (_last_blink + _blink_rate < millis()) {
          if (_blink_state) { // off blink
              // TODO:  handle initial digits 
              Display::_clearDigit(_blink_digit);
          } else { // on blink
              Display::_displayDigit(_blink_digit, _digits[_blink_digit]);
          }
          _last_blink = millis();
          _blink_state = ! _blink_state;
      }
    } else {
      Display::_displayDigit(i, _digits[i]);
    }
  }
}

void Display::dim() {
  tm.set(0);  //set brightness; 0-7(max)
}




void Display::_displayDigit(uint8_t d, uint8_t v) {
  //assert(d <= 3);
  if(v > 9) {
    _clearDigit(d);
  } else {
    tm.display(d,v);
  }
}


void Display::setDigit(uint8_t d, uint8_t v) {
  _digits[d] = v;
}

void Display::displayAllDigits() {
  for(uint8_t i = 0; i < 4; i++) {
    if((i = 0 && _digits[i] == 0) || _digits[i] >= 10 ) { // show 1:00 instead of 01:00 
      tm.clearDigit(0);
    } else {
      _displayDigit(i,_digits[i]);
    }
  }
}

// blinking a new digit will implicitly turn off blink on others
void Display::blinkDigit(uint8_t d, bool onoff) {
  if (onoff) {
    _blink_onoff = true;
    _blink_digit = d;
    _blink_state = false;
  } else {
    _blink_onoff = false;
    _displayDigit(_blink_digit,_digits[_blink_digit]);
  }
  _last_blink = millis();
}

void Display::_clearDigit(uint8_t d) {
  tm.clearDigit(d);
}