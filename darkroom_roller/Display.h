#pragma once
#include "Arduino.h" 
#include "TM1637.h"

// purely static

class Display {
    public:
        static void init();
        static void separator(bool x);
        static void update();
        static void dim();
        static void setDigit(uint8_t d, uint8_t v);
        static void displayAllDigits(bool clear_blink);
        static void blinkDigit(uint8_t d, bool onoff);

        static void _displayDigit(uint8_t d, uint8_t v);
        
    private:
        static TM1637* _disp;
        static void _clearDigit(uint8_t d);
        static uint8_t _digits[4];
        static uint8_t _blink_digit;
        static bool _blink_onoff;
        static bool _blink_state;
        static uint16_t _last_blink;
        static const uint16_t _blink_rate = 150;
};

