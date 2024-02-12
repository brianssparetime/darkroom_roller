#pragma once
#include <AccelStepper.h>
#include "PinsGlobals.h"
#include "Stepper.h"
#include "Arduino.h"
#include "Tilt.h"
#include "Buzzer.h"

// AccelStepper::DRIVER
AccelStepper stepper(1, ST_STEP, ST_DIR);
AccelStepper* Stepper::_stepper = &stepper;
uint16_t Stepper::_cycle_steps = 0;
float Stepper::_ratio = 1;
float Stepper::_target_speed = 0;
bool Stepper::_random = false;
bool Stepper::_wait_to_sleep = false;
bool Stepper::_asleep = false;


#define DEBUG

void Stepper::init() {
   pinMode(ST_SLP, OUTPUT);
   //stepper.setCurrentPosition(0);
   stepper.setMaxSpeed(800);
   stepper.setAcceleration(200); // TESTING -- retry this with good connectors
   _sleep();
   //randomSeed(analogRead(A6));
   randomSeed(3434UL); // TODO replace with unused analog pin -- A6 seems ok
}


void Stepper::go() {
   _awaken();
   if( Tilt::getStatus() ) { 
      _ratio = _big_drum_ratio;
   } else {
      _ratio = _small_drum_ratio;
   }
   _cycle_steps = _get_cycle_steps();
   _target_speed = _ratio * _steps_per_wheel_rotation;
   stepper.setMaxSpeed(_target_speed);


   //stepper.setCurrentPosition(0);
   stepper.move(_cycle_steps); // relative to current position

      #ifdef DEBUG
      Serial.println("Stepper: go");
      // char buf[30];
      // char fbuf[6];
      // // float not supported in sprintf
      // dtostrf(_ratio, 3, 1, fbuf);
      // sprintf(buf, "Stepper: go (ratio = %s)", fbuf);
      // Serial.println(buf);
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

   if(_asleep) {
      return;
   }

   stepper.run();

   // check for sleep
   if(_wait_to_sleep && ! stepper.isRunning()) {
      stepper.disableOutputs();
      delay(20);
      digitalWrite(ST_SLP, LOW); // put driver to sleep
      _wait_to_sleep = false;
      _asleep = true;

      #ifdef DEBUG
         char buf[24];
         sprintf(buf, "Stepper: sleeping...");
         Serial.println(buf);
      #endif DEBUG
   }


   // if one cycle of rotations is complete...
   if (stepper.distanceToGo() < 5) {

      Buzzer::buzz(BUZZ_S);

      #ifdef DEBUG
         Serial.println("Stepper: cycle");
      #endif DEBUG

      //stepper.setCurrentPosition(0); // this sets speed to zero as a side effect
      stepper.setMaxSpeed(-_target_speed); 
      _cycle_steps = _get_cycle_steps();
      stepper.move(_cycle_steps);
   }
}

void Stepper::_sleep() {
   if( ! _asleep) {
      _wait_to_sleep = true;
   }

   #ifdef DEBUG
      char buf[24];
      sprintf(buf, "Stepper:sleep");
      Serial.println(buf);
   #endif DEBUG
}


void Stepper::_awaken() {
   digitalWrite(ST_SLP, HIGH); // awaken driver 
   delay(100); // to let A4988 reawaken
   stepper.enableOutputs();
   _asleep = false;
   _wait_to_sleep = false;

   #ifdef DEBUG
      char buf[24];
      sprintf(buf, "Stepper:awaken");
      Serial.println(buf);
   #endif DEBUG
}

void Stepper::stop() {
   stepper.stop();
   stepper.runToPosition(); // this blocks until stopped
   _sleep();
}


// https://hackaday.io/project/183279-accelstepper-the-missing-manual/details