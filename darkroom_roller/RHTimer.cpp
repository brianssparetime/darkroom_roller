#pragma once
#include "RHTimer.h"
#include "Arduino.h"
#include "PinsGlobals.h"
#include "Machine.h"
#include "UI_States.h"

// when uncommented, this causes watering activation each minute for rapid testing
//#define WARP_SPEED
#define DEBUG

unsigned long RHTimer::_target_time = 0;
uint16_t RHTimer::_cur_interval = Globals::default_interval;
bool RHTimer::_millis_overflow = false;
bool RHTimer::_disabled = true;

void RHTimer::start() {
    _disabled = false;
    unsigned long now = millis();
    _target_time = now + (unsigned long)_cur_interval * 1000UL;
    
    // check for overflow!
    if(_target_time < now) { 
        _millis_overflow = true;
        #ifdef DEBUG
          Serial.println("_millis_overflow true");
        #endif DEBUG
    } else { 
        _millis_overflow = false;
    }

    #ifdef DEBUG
      Serial.println("timer start");
    #endif DEBUG
}

void RHTimer::start(uint16_t interval) {
    _cur_interval = interval; // seconds
    start();
}


void RHTimer::update() {
    if(_disabled) {
        return;
    }

    // if the interval caused target_time to wrap around....
    // check if now has also wrapped and unset flag
    // in other words, this blocks checking target time until both it and millis have wrapped
    if(_millis_overflow) {
        if(millis() <= _target_time) { 
            _millis_overflow = false;
        }

    } else {
        if(millis() > _target_time) {
            alarm();
        }
    }
}

uint16_t RHTimer::get_current_interval() {
    return _cur_interval;
}

uint16_t RHTimer::get_s_remaining() {
    if(_disabled) {
        return 0;
    }
    if(_millis_overflow) {
        // TODO check if this actually works

        // overflow looks like
        // |---t---n---|
        // so instead of going from n to t, we can measure the inside and convert
        //  n - t captures the inside, but we can invert selection to outside by
        // binary NOT (because binary of both sides should combine to 1111) and adding 1.

        unsigned long d = millis() - _target_time;
        d = ~d; // bitwise NOT
        d++;
        return (uint16_t) d / 1000UL;
    } else {
        return (uint16_t) ((_target_time - millis()) / 1000UL);
    }
}

void RHTimer::disable() {
    _disabled = true;
}

// TODO:  implement warning beeps


void RHTimer::alarm() {
    #ifdef DEBUG
        Serial.println("==========ALARM=================="); 
    #endif DEBUG
    disable();
    Stepper::stop();
    Stepper::sleep(true);
    //Display::zeroes();
    // TODO consider flashing all 3 multiplier LEDS for visual!
    // Reminder:  buzzer and led actions persist across states!
        // buzz in new state  for now
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}
