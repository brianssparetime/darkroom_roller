#pragma once
#include "Arduino.h"



class Tilt {
    public:
        static void init();
        static void update();
        static bool getStatus();
    private:
        static long unsigned _last_debounce;
        static bool _state;
        static bool _last_r;

};


