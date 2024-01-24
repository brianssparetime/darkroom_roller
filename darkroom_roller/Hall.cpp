#include "Arduino.h"
#include "Stepper.h"
#include "Hall.h"
#include "Buzzer.h"



volatile long unsigned Hall::_last_debounce = 0;
volatile bool Hall::_rise = false;


void Hall::init() {
    pinMode(HALL, INPUT); // high active
    attachInterrupt(digitalPinToInterrupt(HALL), start_isr, RISING);
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
    _rise = true;
}


/* 

    TODO:  implement something that adds a random delay so reverse isn't happening
    at the same spot on the film all the time

*/


void Hall::update() {
    if(_rise) {
        _rise = false;
        Stepper::rotation();
        Buzzer::buzz(BUZZ_S);
    }

}


