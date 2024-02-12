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

        // for Nema 17 + a4988 / drv8825 at full stepping
        static const uint16_t _steps_per_wheel_rotation = 200; // in steps/sec


    private:
        static void _sleep(bool b);
        static uint16_t _get_cycle_steps();
        static AccelStepper* _stepper;
        // speeds > 1000 are unreliable (note from AccelStepper docs)
        static bool _random; 
        static bool _wait_to_sleep; 
        static bool _asleep; 


        // adjust based on tube size
        // wheel size is 84 mm (3.3″) in diameter, 10.37" in circumference
        // small drum is 4 inches in diameter, about 12.57 circumference, 
        // large drum is 6 inches in diameter, about 18.85
        // target RPM is about 50-75pm, which are speeds offered on jobo
        static float _ratio;
        static const float _big_drum_ratio = 1.8;
        static const float _small_drum_ratio = 1.2;
        static const uint8_t _cycle_drum_rotations = 1;

        static const float _max_speed = 800;
        static float _target_speed;
        static uint16_t _cycle_steps;
};

