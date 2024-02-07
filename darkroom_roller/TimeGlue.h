#pragma once
#include "Arduino.h"

// purely static

class TimeGlue {
    public:
        static void TimeGlue::displayInterval(uint16_t seconds);
        static void TimeGlue::displayMSS(int8_t mss[]);
        static uint16_t TimeGlue::MSStoInterval(int8_t mss[]);

    private:

};