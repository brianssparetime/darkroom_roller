#pragma once
#include "Arduino.h"

// to match PCB from darkroom timer

// LCD uses I2C SDA / SCL on A4 and A5

// rotary encoder
#define RE_BUT_PIN 6
#define RE_A_PIN 7
#define RE_B_PIN 8

//empty sensor, indicator
//#define LOW_SENSOR_PIN 5
//pinmode(LOW_SENSOR_PIN, INPUT);

//buzzer
#define BUZZER_PIN 9
#define BUZZ_S 100
#define BUZZ_M 250
#define BUZZ_L 500
#define BUZZ_XL 1500

// stepper
#define ST_DIR A0
#define ST_STEP  A5
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
#define MAX_INTERVAL 3600 // seconds





namespace Globals {
    //uint8_t maxes at 255

    static const uint8_t default_interval = 60; // seconds

}
