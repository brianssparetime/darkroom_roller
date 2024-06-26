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

        
        static const uint8_t _step_size = 1; // 1/x, where 1 is fulls teps, 2 is half, 4 is 1/4 stepping...

        // for Nema 17 + a4988 / drv8825 at full stepping
        static const uint16_t _steps_per_wheel_rotation = 200 * _step_size; // in steps/sec


    private:
        static void _sleep();
        static void _awaken();
        static int16_t _get_cycle_steps();
        static AccelStepper* _stepper;
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
        static const uint8_t _cycle_drum_rotations = 3;

        // speeds > 1000 are unreliable (note from AccelStepper docs)
        static const float _max_speed = 800;
        static float _target_speed;
        static int16_t _cycle_steps;

        // debug 
        static uint8_t iii;
};

