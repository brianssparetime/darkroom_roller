#pragma once
#include "Arduino.h"
#include <AccelStepper.h>


// purely static

class Stepper {
    public:
        static void init();
        static void update();
        static void go();
        static void stop();
        static void sleep(bool b);
        static void reverse();
        static void rotation();

    private:
        static AccelStepper* _stepper;
        static const float _max_speed = 128; // TODO test and fixme
        // speeds > 1000 are unreliable (note from AccelStepper docs)
        static const uint8_t _rotations_before_reverse = 3;
        static uint8_t _rotations;
};

