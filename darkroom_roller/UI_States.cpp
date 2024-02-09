#pragma once
#include "UI_States.h"

#include "Arduino.h"
#include "Machine.h"
#include "RHTimer.h"
#include "Buzzer.h"
#include "Stepper.h"
#include "Display.h"
#include "PinsGlobals.h"
#include "TimeGlue.h"



/*********** UI_State *************/

// methods implemented here can be omitted from classes inheriting UI_State below
// (as can constructors and destructors), as long as the implementions here match the
// .h header file

// these methods don't get invoked unless a) not defined for the inheriting class, or 
// b) explicitly called by inheriting class


void UI_State::update() {
    // if( millis() - Machine::get_last_action() > UI_State::_inactive_timeout ) {
    //     #ifdef DEBUG
    //     Serial.println("going inactive!");
    //     #endif
    //     //Machine::changeState(static_cast<UI_State *>(new UI_Inactive()));
    // } 
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
#define DEBUG

void UI_Welcome::activate() {
    #ifdef DEBUG
      Serial.println(F("UI Welcome activated"));
    #endif
    _start = millis();
    Buzzer::buzz(BUZZ_M);
    TimeGlue::zeroes();
    //Leds::all_on();

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


uint8_t UI_Interval_Set::_edit_values[] = {1,0,0};  // mm ss s

void UI_Interval_Set::activate() {
    Buzzer::buzz(BUZZ_L);
    _edit_digit = 0; // mins
    TimeGlue::displayMSS(_edit_values);
    #ifdef DEBUG
      Serial.println("UI Interval Set activated");
    //   char buf[16];
    //   sprintf(buf, "nis = %3d", _new_interval_selected);
    //   Serial.println(buf);
    #endif
    //Leds::update();
}

void UI_Interval_Set::handle_button_press() {
    #ifdef DEBUG
      Serial.println("UI Interval Set:  short press");
    #endif
    //Leds::nextMult();
    //Leds::update();
    if (_edit_digit >= 2) {
        _edit_digit = 0;
    } else {
        _edit_digit++;
    }
    Display::blinkDigit(_edit_digit+1,true);
    Buzzer::buzz(BUZZ_S);
}

void UI_Interval_Set::handle_button_long_press() {
    #ifdef DEBUG
      Serial.println("UI Interval Set long press:");
      //char buf[24];
      //sprintf(buf, "new interval = %3d", _new_interval_selected);
      //Serial.println(buf);
    #endif
    // buzz in the new state
    RHTimer::start(TimeGlue::MSStoInterval(_edit_values));
    delay(600); 
    Machine::changeState(static_cast<UI_State *>(new UI_Active()));
}

void UI_Interval_Set::handle_rotation(int delta) {
    // if (delta == 0) {
    //     return;
    // }
    int8_t oldv = _edit_values[_edit_digit];
    int8_t dir = (delta < 0) ? 1 : -1;

    // NOTE:  C++ % operator can return negatives, so using
        // return (i % n + n) % n;
    // instead to get a positive-only modulo


    switch( _edit_digit ) {
        case 0: // minutes, 0-14:59 allowed
            _edit_values[_edit_digit] = ((oldv + dir) % 14 + 14) % 14;
            break;
        case 1: // tens mins, 0-5 allowed
            _edit_values[_edit_digit] = ((oldv + dir) % 6 + 6) % 6;
            break;
        case 2: // ones mins, 0-9 allowed
            _edit_values[_edit_digit] = ((oldv + dir) % 10 + 10) % 6;
            break;
    }

     #ifdef DEBUG
      Serial.println("UI Interval Set rotation:");
      char buf[24];
      sprintf(buf, "set digit %3d to %3d", _edit_digit, _edit_values[_edit_digit]);
      Serial.println(buf);
    #endif 

    TimeGlue::displayMSS(_edit_values);
    Buzzer::buzz(BUZZ_S);
}

void UI_Interval_Set::update() {
    //Leds::update();
    // buzzer updates from main()
    // no need to update stepper, hall, or timer here

    //TimeGlue::displayInterval(RHTimer::get_s_remaining());
    //TimeGlue::displayMSS(_edit_values);
    Display::update();
}

/*********** UI_Active *************/


void UI_Active::activate() {
    #ifdef DEBUG
      Serial.println("UI Active activated");
    #endif
    Buzzer::buzz(BUZZ_L);
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

void UI_Active::update() {
    TimeGlue::displayInterval(RHTimer::get_s_remaining());
    Stepper::update();
    #ifdef DEBUG
      Serial.println("UI Active update():");
    #endif
    //Leds::update(); -- no need for this
    RHTimer::update(); 
    /* note RHTimer::update() can cause state change, 
    so should come last to avoid any following code executing in wrong state post change*/
}

// turn off before exiting state
UI_Active::~UI_Active() {
    Stepper::stop();
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

// void UI_Inactive::activate() {
//     #ifdef DEBUG
//       Serial.println("UI Inactive activated");
//     #endif
//     Buzzer::buzz(BUZZ_S);
//     Display::dim();
//     Leds::off();
// }

// void UI_Inactive::handle_button_press() {
//     Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
// }
// void UI_Inactive::handle_rotation(int delta) { 
//     Machine::changeState(static_cast<UI_State *>(new UI_Interval_Set()));
// }

// void UI_Inactive::update() {
//     // Note:  because this doesn't explicitly call UI_State::update(), that method is overridden
//     // and not invoked from here, so we don't have to worry about constantly changing to Inactive
// }

