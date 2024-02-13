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
int16_t Stepper::_cycle_steps = 0;
float Stepper::_ratio = 1;
float Stepper::_target_speed = 0;
bool Stepper::_random = false;
bool Stepper::_wait_to_sleep = false;
bool Stepper::_asleep = false;
uint8_t Stepper::iii = 0;


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
   iii = 0;
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


int16_t Stepper::_get_cycle_steps() {
   int16_t cycle_steps = _cycle_drum_rotations * _ratio * _steps_per_wheel_rotation;

   // if (_random) {
   // cycle_steps += ( random(0,cycle_steps - 15) - (cycle_steps / 2));
   // }

   // #ifdef DEBUG
   //    char buf[48];
   //    sprintf(buf, "Stepper:set_cycle_steps - steps set to %d", cycle_steps);
   //    Serial.println(buf);
   //  #endif 

    return cycle_steps;
}



void Stepper::update() {

   if(_asleep) {
      return;
   }

   if ( iii % 16 == 0) {
      #ifdef DEBUG
         char buf[64];
         sprintf(buf, "Stepper:update cp = %ld tp = %ld dtg = %ld", \
             stepper.currentPosition(), stepper.targetPosition(), stepper.distanceToGo());
             // all three are signed longs
         Serial.println(buf);
      #endif 

   }
   iii ++; // overflow intentional


   stepper.run();


   // if one cycle of rotations is complete...
   if (!stepper.isRunning()) {

      Buzzer::buzz(BUZZ_S);

      #ifdef DEBUG
         Serial.println("Stepper: cycle");
      #endif DEBUG

      //stepper.setCurrentPosition(0); // this sets speed to zero as a side effect
      //stepper.setMaxSpeed(_target_speed); 
      _cycle_steps = 0L - _cycle_steps;
      stepper.move(_cycle_steps);
   }
}




void Stepper::_awaken() {
   digitalWrite(ST_SLP, HIGH); // awaken driver 
   delay(100); // to let A4988 reawaken
   stepper.enableOutputs();
   _asleep = false;
   _wait_to_sleep = false;

   #ifdef DEBUG
      char buf[24];
      sprintf(buf, "Stepper: awaken");
      Serial.println(buf);
   #endif DEBUG
}

void Stepper::_sleep() {
   delay(20);
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


void Stepper::stop() {
   #ifdef DEBUG
      Serial.println("Stepper: stopping");
   #endif DEBUG
   stepper.stop();
   stepper.runToPosition(); // this blocks until stopped
   _sleep();
   #ifdef DEBUG
      Serial.println("Stepper: fully stopped");
   #endif DEBUG
}


// https://hackaday.io/project/183279-accelstepper-the-missing-manual/details