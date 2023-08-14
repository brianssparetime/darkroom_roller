#pragma once
#include "Arduino.h"
#include "Stepper.h"
#include "PinsGlobals.h"



class Hall {
    public:
        static void init();
        static void start_isr();
        static void update();
    private:
        static volatile long unsigned _last_debounce;
        static volatile bool _new_dir;
        static bool _dir;
};


