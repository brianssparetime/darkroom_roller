#pragma once
#include "Arduino.h" 
#include "TM1637.h"

// purely static

class Display {
    public:
        static void init();
        static void update();
        static void zeroes();
        static void separator(bool x);
        static void display(uint16_t s);
        static void dim();
        static void clearDigit(uint8_t d);
        static void displayDigit(uint8_t d, uint8_t v);
    private:
        static TM1637* _disp;
        static void displayTimeSeg(uint16_t seconds);
};

