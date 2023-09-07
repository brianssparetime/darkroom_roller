#pragma once
#include "Arduino.h"

//#define DEBUG

// rotary encoder
#define RE_BUT_PIN 6
#define RE_A_PIN 7
#define RE_B_PIN 8


//buzzer
#define BUZZER_PIN 9


// stepper
#define ST_DIR A0
#define ST_STEP  10
#define ST_SLP 5

// hall
#define HALL A1

// 4char7seg
#define DISP_CLK A2
#define DISP_DIO A3

// leds
#define LED_A 2
#define LED_B 3
#define LED_C 4
#define NUM_LEDS 3


#define MAX_INTERVAL 3600 // 1 hour; in seconds


namespace Globals {
    //uint8_t maxes at 255

    static const uint8_t default_interval = 60; // seconds

}
