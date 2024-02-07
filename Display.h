#pragma once
#include "Arduino.h" 
#include "TM1637.h"

// purely static

class Display {
    public:
        static void init();
        static void separator(bool x);
        static void update();
        static void zeroes();
        static void displayTime(uint8_t hours, uint8_t mins);
        static void dim();
        static void setDigit(uint8_t d, uint8_t v);
        static void displayAllDigits();

    private:
        static TM1637* _disp;
        static void _displayDigit(uint8_t d, uint8_t v);
        static void _clearDigit(uint8_t d);
        static uint8_t _digits[4];
        static uint8_t _blink_digit;
        static bool _blink_onoff;
        static bool _blink_state;
        static uint16_t _last_blink;
        static const uint16_t _blink_rate = 150;
};

