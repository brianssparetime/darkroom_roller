#pragma once
#include "Arduino.h"
#include "Machine.h"
#include "RHTimer.h"
#include "Hall.h"
#include "Buzzer.h"
#include "Stepper.h"
#include "Display.h"
#include "PinsGlobals.h"

class UI_State {
    public:
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_button_long_press();
        virtual void handle_button_long_release();
        virtual void handle_rotation(int delta);
    private:
        static const unsigned _inactive_timeout = 5 * 1000; 
        // unsigned int maxes at 65535

};


// class UI_State_Setter {
//     public:
//         static int next_left(int current, uint8_t arr[], uint8_t arr_size);
//         static int next_right(int current, uint8_t arr[], uint8_t arr_size);
// };

class UI_Welcome : public UI_State {
    public:
        virtual void activate();
        virtual void update();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
    private:
        const uint8_t _duration = 2; // seconds
        unsigned long _start;
        

};


class UI_Interval_Set : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_button_long_press();
        virtual void handle_rotation(int delta);
        virtual void update();
    private:
        uint16_t _new_interval_selected;
    
};

class UI_Active : public UI_State {
    public:
        virtual void activate();
        virtual void handle_button_press();
        virtual void handle_rotation(int delta);
        virtual void update();
        virtual ~UI_Active();
};


// class UI_Inactive : public UI_State {
//     public:
//         virtual void activate();
//         virtual void handle_button_press();
//         virtual void handle_rotation(int delta);
//         virtual void update();
//     private:
//         unsigned long _start;
// };
