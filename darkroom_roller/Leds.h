#pragma once
#include "Arduino.h"
#include "PinsGlobals.h"


class Leds {
    public:
        static void init();
        static void update();
        static uint8_t getMult();
        static void nextMult();
        static void off();
        static void all_on();
    private:
        static uint8_t _mult_idx; // unit multiplier
        static const uint8_t _mult_choices[NUM_LEDS];
        static const uint8_t _pins[NUM_LEDS];
};