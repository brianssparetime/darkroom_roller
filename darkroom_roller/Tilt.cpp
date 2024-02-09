#include "Arduino.h"
#include "Stepper.h"
#include "Tilt.h"
#include "Buzzer.h"
#include "PinsGlobals.h"



long unsigned Tilt::_last_debounce = 0;
bool Tilt::_state = false;
bool Tilt::_last_r = false;


void Tilt::init() {
    pinMode(HALL, INPUT); // high active
}



void Tilt::update() {
    unsigned long now = millis();
    bool r = digitalRead(HALL);

    if(r != _last_r) {
        _last_debounce = now;
    }

    if((now - _last_debounce) > 50) {
        if (r != _state) {
            _state = r;
        }
        _state = ! _state;
    }
    _last_r = r;
}

bool Tilt::getStatus() {
    return _state;
}

