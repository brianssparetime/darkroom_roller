#include "Arduino.h"
#include "PinsGlobals.h"
#include "Leds.h"


const uint8_t Leds::_pins[NUM_LEDS] = {LED_A, LED_B, LED_C};
const uint8_t Leds::_mult_choices[NUM_LEDS] = {1,5,60};
uint8_t Leds::_mult_idx= 1; // unit multiplier, default 10s


void Leds::init() {
    _mult_idx= 1; // unit multiplier, default 10s
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        pinMode(_mult_choices[i], OUTPUT);
    }
    update();
}


void Leds::update() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        if (i == _mult_idx) {
            digitalWrite(i, HIGH);
        } else {
            digitalWrite(i,LOW);
        }
    }
}


uint8_t Leds::getMult() {
    return _mult_choices[_mult_idx];
}

void Leds::nextMult() {
    if (_mult_idx + 1 == NUM_LEDS) {
        _mult_idx = 0;
    } else {
        _mult_idx++;
    }
}

void Leds::all_on() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        digitalWrite(i, HIG);
    }
}
void Leds::off() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        digitalWrite(i, LOW);
    }
}