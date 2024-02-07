#include "Arduino.h"
#include "Stepper.h"
#include "Tilt.h"
#include "Buzzer.h"
#include "PinsGlobals.h"



long unsigned Tilt::_last_debounce = 0;
bool Tilt::_state = false;


void Tilt::init() {
    pinMode(HALL, INPUT); // high active
}



void Tilt::update() {
    if(digitalRead(HALL) != _state) {
        unsigned long now = millis();
        if (now - _last_debounce < 50) {
            _last_debounce = now;
            return;
        }
        _last_debounce = now;
        _state = ! _state;
    }
}

bool Tilt::getStatus() {
    return _state;
}

