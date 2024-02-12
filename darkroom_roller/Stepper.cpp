#pragma once
#include <AccelStepper.h>
#include "PinsGlobals.h"
#include "Stepper.h"
#include "Arduino.h"
#include "Tilt.h"
#include "Buzzer.h"


AccelStepper stepper(Stepper::_steps_per_wheel_rotation, ST_STEP, ST_DIR);
AccelStepper* Stepper::_stepper = &stepper;
uint8_t Stepper::_cycle_steps = 0;
float Stepper::_ratio = 1;
float Stepper::_actual_speed = 0;
bool Stepper::_random = false;


#define DEBUG

void Stepper::init() {
   pinMode(ST_SLP, OUTPUT);
   stepper.setMaxSpeed(800);
   //stepper.setAcceleration(20); // TESTING -- retry this with good connectors
   _sleep(true);
   //randomSeed(analogRead(A6));
   randomSeed(3434UL); // TODO replace with unused analog pin -- A6 seems ok
}


void Stepper::go() {
   if( Tilt::getStatus() ) { 
      _ratio = _big_drum_ratio;
   } else {
      _ratio = _small_drum_ratio;
   }
   _cycle_steps = _get_cycle_steps();
   _actual_speed = _ratio * _steps_per_wheel_rotation;
   _sleep(false); // awaken
   //stepper.setCurrentPosition(0);
   stepper.setSpeed(_actual_speed);
   stepper.runSpeed();

      #ifdef DEBUG
         Serial.println("Stepper: go");
      #endif DEBUG
}


uint16_t Stepper::_get_cycle_steps() {
   uint16_t cycle_steps = _cycle_drum_rotations * _ratio * _steps_per_wheel_rotation;

   if (_random) {
   cycle_steps += ( random(0,cycle_steps - 15) - (cycle_steps / 2));
   }

   #ifdef DEBUG
      char buf[48];
      sprintf(buf, "Stepper:set_cycle_steps - steps set to %d", cycle_steps);
      Serial.println(buf);
    #endif 

    return cycle_steps;
}



void Stepper::update() {
   stepper.runSpeed();

   /*
   // if one cycle of rotations is complete...
   if (stepper.currentPosition() > _cycle_steps) {

      Buzzer::buzz(BUZZ_S);

      #ifdef DEBUG
         Serial.println("Stepper: cycle");
      #endif DEBUG

      //stepper.setCurrentPosition(0); // this sets speed to zero as a side effect
      stepper.setSpeed(-_actual_speed); 
      _cycle_steps = _get_cycle_steps();
      stepper.runSpeed();
   }
   */
}

void Stepper::_sleep(bool b) {
   if(b) {
      delay(100); // to let A4988 reawaken
      digitalWrite(ST_SLP, LOW); // put driver to sleep
   } else {
      digitalWrite(ST_SLP, HIGH); // awaken driver 
      delay(100); // to let A4988 reawaken
   }
}

void Stepper::stop() {
   stepper.stop();
   _sleep(true);
}


