#include "Arduino.h"
#include "Stepper.h"
#include "Hall.h"



volatile long unsigned Hall::_last_debounce = 0;
volatile bool Hall::_new_dir = false;
bool Hall::_dir = false;


void Hall::init() {
    pinMode(HALL, INPUT); // high active
    attachInterrupt(digitalPinToInterrupt(HALL), Hall::start_isr, RISING);
}



void Hall::start_isr() {

  // within the ISR and any functions it calls, time (as seen by delay() and millis() is frozen
  // you can call them, but they don't advance
  // Also, don't println from here...
  // https://forum.arduino.cc/t/delay-and-interrupts/405046
  
    unsigned long now = millis();
    if (now - _last_debounce < 50) {
        _last_debounce = now;
        return;
    }
    _last_debounce = now;
    _new_dir = !_new_dir;
}

void Hall::update() {
    if(_dir != _new_dir){
        _dir = _new_dir;
        Stepper::reverse();
    }

}


