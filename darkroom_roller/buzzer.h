#pragma once
#include "Arduino.h"

#define BUZZ_S 100 // ms
#define BUZZ_M 250
#define BUZZ_L 500
#define BUZZ_XL 1500

enum buzzmode {SINGLE, CONT, SEQ};

class Buzzer {
    public:
        static void init(int pin);
        static void buzz();
        static void buzz(uint16_t duration_ms);
        static void buzz_indef(uint16_t ontime, uint16_t offtime);
        static void buzz_seq(uint16_t ontime, uint16_t offtime, uint8_t reps);
        static void cancel();
        static void update();
    private:
        static uint8_t _pin;
        static uint16_t _stop_time;
        static uint16_t _ontime;
        static uint16_t _offtime;
        static uint8_t _reps;
        static const uint16_t _default_duration = 500;

};