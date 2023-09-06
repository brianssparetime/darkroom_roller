#pragma once
#include "Arduino.h"

// purely static

class RHTimer {
    public:
        static void start();
        static void start(uint16_t interval);
        static uint16_t get_s_remaining();
        static void update();
        static void disable();
        static uint16_t get_current_interval();

    private:

        static void alarm();

        static bool _millis_overflow;
        static unsigned long _target_time;
        static uint16_t _cur_interval;
        static bool _disabled;

};