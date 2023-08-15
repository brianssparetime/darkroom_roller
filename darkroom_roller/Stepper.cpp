#pragma once
#include <AccelStepper.h>
#include "PinsGlobals.h"
#include "Stepper.h"
#include "Arduino.h"


AccelStepper stepper(1, ST_STEP, ST_DIR);
AccelStepper* Stepper::_stepper = &stepper;
uint8_t _rotations = 0;


void Stepper::init() {
   pinMode(ST_SLP, OUTPUT);
   stepper.setMaxSpeed(_max_speed);
   stop();
   sleep(true);
}


void Stepper::update() {
   stepper.runSpeed();
}

void Stepper::sleep(bool b) {
   if(b) {
      digitalWrite(ST_SLP, LOW); // put driver to sleep
   } else {
      digitalWrite(ST_SLP, HIGH); // awaken driver 
   }
}


void Stepper::go() {
   sleep(false); // awaken
   delay(2); // to let A4988 reawaken
   stepper.setSpeed(_max_speed);
}

void Stepper::stop() {
   stepper.setSpeed(0);
   // consider putting sleep true here
}

void Stepper::rotation() {
   _rotations++;
   if(_rotations == _rotations_before_reverse) {
      reverse();
   }
}

void Stepper::reverse() {
   stepper.setSpeed(-_max_speed);
}