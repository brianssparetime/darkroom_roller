#pragma once
#include "Arduino.h"
#include "Display.h"
#include "TM1637.h"
#include "PinsGlobals.h"



TM1637 tm(DISP_CLK, DISP_DIO); // set up 47seg
TM1637* Display::_disp = &tm;

uint8_t Display::_digits[4] = {1,2,0,0};
uint8_t Display::_blink_digit = 1;
bool Display::_blink_state = false;
bool Display::_blink_onoff = false;
uint16_t Display::_last_blink = 0;


void Display::init() {
    tm.init();
    tm.set(2);  //set brightness; 0-7(max)
    separator(true);
}


void Display::separator(bool x) {
  tm.point(1 ? x : 0);
}

void Display::update() {

  if (! _blink_onoff) {
    return;
  }

  if (_last_blink + _blink_rate < millis()) {
      if (_blink_state) { // off blink
          Display::_clearDigit(_blink_digit);
      } else { // on blink
          Display::_displayDigit(_blink_digit, _digits[_blink_digit]);
      }
      _last_blink = millis();
      _blink_state = ! _blink_state;
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

  #ifdef DEBUG
    char buf[48];
    sprintf(buf, "Display::setDigit  %d to %d", d, v); 
    Serial.println(buf);
  #endif
}

void Display::displayAllDigits(bool clear_blink) {
  if(clear_blink) {
    _blink_onoff = false;
  }
  for(uint8_t i = 0; i < 4; i++) {
    if( i == 0 && _digits[i] == 0 ) { // show 1:00 instead of 01:00 
      tm.clearDigit(0);
    } else {
      _displayDigit(i,_digits[i]);
    }
  }
  #ifdef DEBUG
    char buf[48];
    sprintf(buf, "Display::displayalldigits  %d %d %d %d", _digits[0], _digits[1], _digits[2], _digits[3]);
    Serial.println(buf);
  #endif
}

// blinking a new digit will implicitly turn off blink on others
void Display::blinkDigit(uint8_t d, bool onoff) {
  displayAllDigits(false); // make sure if some other digit was blinked off, it's now on
  if (onoff) {
    _blink_onoff = true;
    _blink_digit = d;
    _blink_state = false;
    _clearDigit(d);
  } else {
    _blink_onoff = false;
    _displayDigit(_blink_digit,_digits[_blink_digit]);
  }
  _last_blink = millis();
}

void Display::_clearDigit(uint8_t d) {
  tm.clearDigit(d);
}