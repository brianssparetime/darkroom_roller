#pragma once
#include <AccelStepper.h>
#include "PinsGlobals.h"
#include "Stepper.h"
#include "Arduino.h"
#include "Tilt.h"


AccelStepper stepper(1, ST_STEP, ST_DIR);
AccelStepper* Stepper::_stepper = &stepper;
float Stepper::_cycle_wheel_rotations = 0;
uint8_t Stepper::_cycle_steps = 0;
float Stepper::_ratio = 1;
bool Stepper::_random = false;


#define DEBUG

void Stepper::init() {
   pinMode(ST_SLP, OUTPUT);
   stepper.setMaxSpeed(_max_speed);
   stepper.setCurrentPosition(0);
   //stepper.setAcceleration(20); // TESTING -- retry this with good connectors
   _sleep(true);
   //randomSeed(analogRead(A6));
   randomSeed(3434UL); // TODO replace with unused analog pin -- A6 seems ok
}

void Stepper::_set_cycle_steps() {
   if( Tilt::getStatus() ) { 
      _ratio = _big_drum_ratio;
   } else {
      _ratio = _small_drum_ratio;
   }
   _cycle_wheel_rotations = _cycle_drum_rotations * _ratio;
   _cycle_steps = _cycle_wheel_rotations * _steps_per_wheel_rotation;

   if (_random) {
   _cycle_steps += ( random(0,_cycle_steps - 15) - (_cycle_steps / 2));
   }

   #ifdef DEBUG
      char buf[48];
      sprintf(buf, "Stepper:set_cycle_steps - steps set to %d");
      Serial.println(buf);
    #endif 
}



void Stepper::update() {
   stepper.runSpeed();


   if (stepper.currentPosition() > _cycle_steps) {
      stepper.setCurrentPosition(0);
      stepper.setSpeed(-_max_speed);
      _set_cycle_steps();
   }
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


void Stepper::go() {
   _sleep(false); // awaken
   _set_cycle_steps();
   stepper.setSpeed(_max_speed);
   stepper.runSpeed();
}

void Stepper::stop() {
   stepper.setSpeed(0);
   stepper.runSpeed();
   _sleep(true);
}
