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
        static void rotation();

    private:
        static void _sleep(bool b);
        static void _set_cycle_steps();
        static AccelStepper* _stepper;
        static const float _max_speed = 200; // TODO test and fixme
        // speeds > 1000 are unreliable (note from AccelStepper docs)

        // for Nema 17 + a4988
        static const uint16_t _steps_per_wheel_rotation = 200;

        // adjust based on tube size
        // wheel size is 84 mm (3.3″) in diameter
        static const float _big_drum_ratio = 2;
        static const float _small_drum_ratio = 1.333;
        static float _ratio;
        static const uint8_t _cycle_drum_rotations = 2;

        static float _cycle_wheel_rotations;
        static uint8_t _cycle_steps;
};

