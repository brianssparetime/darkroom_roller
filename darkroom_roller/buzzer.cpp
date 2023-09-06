#pragma once
#include "Arduino.h"
#include "buzzer.h"
#include "Leds.h"
#define LEDS

uint8_t Buzzer::_pin = 0;
uint16_t Buzzer::_stop_time = 0;
uint16_t Buzzer::_ontime = 0;
uint16_t Buzzer::_offtime = 0;
uint8_t Buzzer::_reps = 0;
bool _pin_state = false;
buzzmode _mode = SINGLE;


void Buzzer::init(int pin) {
    _pin = pin;
    _stop_time = 0;
    pinMode(_pin, OUTPUT); // high active
    digitalWrite(_pin, LOW);
}

void Buzzer::buzz() {
    buzz(_default_duration);
}

void Buzzer::buzz(uint16_t duration_ms) {
    cancel();
    #ifdef LEDS
      Leds::all_on();
    #endif
    _stop_time = millis() + duration_ms;
    digitalWrite(_pin, HIGH);
}

void Buzzer::cancel() {
    digitalWrite(_pin, LOW);
    #ifdef LEDS
      Leds::off();
    #endif
    _stop_time = 0;
    _reps = 0;
    _offtime = 0;
    _ontime = 0;
    _mode = SINGLE;
}

void Buzzer::update() {
    
    // TODO:  consider also making this overflow safe
    if(millis() < _stop_time) {
        return;
    }

    if(_mode == SINGLE) {
        cancel();
        return;
    }

    if(_mode == SEQ) {
        _reps -= (uint8_t) !_pin_state; // add 1 when _pin_state is off at end of cycle
        if(_reps == 0) {
            cancel();
            return;
        }
    }

    // seq or cont
    _stop_time = millis() + (_pin_state) ? _offtime : _ontime;
    _pin_state = !_pin_state;
}

void Buzzer::buzz_seq(uint16_t ontime, uint16_t offtime, uint8_t reps) {
    _mode = SEQ;
    _reps = reps;
    _ontime = ontime;
    _offtime = offtime;
    _stop_time = millis() + ontime;
    _pin_state = true;
    digitalWrite(_pin, HIGH);
}

void Buzzer::buzz_indef(uint16_t ontime, uint16_t offtime) {
    buzz_seq(ontime, offtime, (uint8_t) 0);
    _mode = CONT;
}
