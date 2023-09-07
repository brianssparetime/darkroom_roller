#pragma once
#include "UI_States.h"

#include "Arduino.h"
#include "Machine.h"
#include "RHTimer.h"
#include "Buzzer.h"
#include "Stepper.h"
#include "Display.h"
#include "PinsGlobals.h"
#include "Leds.h"


/*********** UI_State *************/

// methods implemented here can be omitted from classes inheriting UI_State below
// (as can constructors and destructors), as long as the implementions here match the
// .h header file

// these methods don't get invoked unless a) not defined for the inheriting class, or 
// b) explicitly called by inheriting class


void UI_State::update() {
    if( millis() - Machine::get_last_action() > UI_State::_inactive_timeout ) {
        #ifdef DEBUG
        Serial.println("going inactive!");
        #endif
        Machine::changeState(static_cast<UI_State *>(new UI_Inactive()));
    } 
}


void UI_State::handle_button_press() { }
void UI_State::handle_button_long_press() { }
void UI_State::handle_button_long_release() { }
void UI_State::handle_rotation(int delta) { }

// /*********** UI_State_Setter *************/


// static int UI_State_Setter::next_left(int current, uint8_t arr[], uint8_t arr_size) {
    
//     // on match, return holder, which is either previous 
//     // or if i=0, then return holder (which is already set to the last)
//     int p = arr[arr_size - 1];
//     for (uint8_t i = 0; i < arr_size; i++) {
//         int x = arr[i];
//         if(x == current) {
//             return p;
//         }
//         p = x;
//     }
// }

// static int UI_State_Setter::next_right(int current, uint8_t arr[], uint8_t arr_size) {
//     // if we're currently at the last position, return first one
//     if (arr[arr_size - 1] == current) {
//         return arr[0];
//     }
//     // otherwise, start from begining, go until second to last, and return next after match
//     for (uint8_t i = 0; i < arr_size - 1; i++) {
//         int x = arr[i];
//         if(x == current) {
//             return arr[i+1];
//         }
//     }
// }


/*********** UI_Welcome *************/


void UI_Welcome::activate() {
    #ifdef DEBUG
      Serial.println(F("UI Welcome activated"));
    #endif
    _start = millis();
    Buzzer::buzz(BUZZ_M);
    Display::zeroes();

}

void UI_Welcome::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}
void UI_Welcome::handle_rotation(int delta) { 
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

void UI_Welcome::update() { 
    unsigned long now = millis();
    if(now - _start > 1000UL * _duration) {
        Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
    }
}


/*********** UI_Interval_Set *************/


void UI_Interval_Set::activate() {
    #ifdef DEBUG
      Serial.println("UI Interval Set activated");
    #endif
    uint16_t ci = RHTimer::get_current_interval();
    Display::display(ci);
    _new_interval_selected = ci;
    Buzzer::buzz(BUZZ_XL);
}

void UI_Interval_Set::handle_button_press() {
    #ifdef DEBUG
      Serial.println("UI Interval Set:  short press");
    #endif
    Leds::nextMult();
    Leds::update();
    Buzzer::buzz(BUZZ_S);
}

void UI_Interval_Set::handle_button_long_press() {
    #ifdef DEBUG
      // TODO:  get rid of String()
      Serial.println("UI Interval Set long press:  new interval set to " + String(_new_interval_selected));
    #endif
    // buzz in the new state
    RHTimer::start(_new_interval_selected);
    Machine::changeState(static_cast<UI_State *>(new UI_Active()));
}

void UI_Interval_Set::handle_rotation(int delta) {
    uint16_t m = Leds::getMult();
    if(delta > 0) {
        if (_new_interval_selected + m > MAX_INTERVAL) { // selection greater than max
            Buzzer::buzz(BUZZ_L);
        } else {
            Buzzer::buzz(BUZZ_S);
            _new_interval_selected += m;
        }
    } else if (delta < 0) {
        if(m > _new_interval_selected) {
            _new_interval_selected = 0;
            Buzzer::buzz(BUZZ_L);
        } else {
            _new_interval_selected -= m;
            Buzzer::buzz(BUZZ_S);
        }
    }
    Display::display(_new_interval_selected);
}

// inherit update, which allows going inactive


/*********** UI_Active *************/


void UI_Active::activate() {
    #ifdef DEBUG
      Serial.println("UI Active activated");
    #endif
    Buzzer::buzz(BUZZ_XL);
    Stepper::go();
}

void UI_Active::handle_button_press() { 
        Stepper::stop();
        Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}
void UI_Active::handle_rotation(int delta) { 
        Stepper::stop();
        Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

// disable going inactive!
void UI_Active::update() {
    RHTimer::update();
    Stepper::update();
    Display::display(RHTimer::get_s_remaining());
    //Leds::update();
    Hall::update();
}

// turn off before exiting state
UI_Active::~UI_Active() {
    Stepper::stop();
    Stepper::sleep(true);
}



/*********** UI_Inactive *************/

// static uint8_t UI_Inactive::get_time_left(char* sb) {
//     uint8_t d = RHTimer::get_d_remaining();
//     uint8_t h = RHTimer::get_h_remaining();
//     uint8_t m = RHTimer::get_m_remaining();
//     uint8_t s = RHTimer::get_s_remaining();
//     uint8_t v = RHTimer::get_current_interval();
//     if(d > 0) {
//         sprintf(sb, "%dd%dh / %02dh", d, h, v);
//         return h;
//     } else {
//         sprintf(sb, "%dh%02dm%02ds / %02dh", h, m, s, v);
//         return s;
//     }
// }

void UI_Inactive::activate() {
    #ifdef DEBUG
      Serial.println("UI Inactive activated");
    #endif
    Buzzer::buzz(BUZZ_S);
    Display::dim();
    Leds::off();
    Stepper::sleep(true);
}

void UI_Inactive::handle_button_press() {
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}
void UI_Inactive::handle_rotation(int delta) { 
    Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
}

void UI_Inactive::update() {
    // Note:  because this doesn't explicitly call UI_State::update(), that method is overridden
    // and not invoked from here, so we don't have to worry about constantly changing to Inactive
}

