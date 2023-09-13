# Darkroom Roller
Arduino Nano project to drive a stepper motor on a timer, for making darkroom prints with drums



## Usage:
 - turn rotary encoder to increment / decrement timer
 - short push rotary encoder to toggle between increment/decrement amounts
 - long press rotary encoder to start timer, causing stepper to run
 - hall effect sensor to count rotations and reverse after a few

## Components:

### Electronics:
 - TM1637 4 digit 7 segment display
 - piezo buzzer
 - rotary encoder
 - hall effect sensor
 - A4988 or compatible stepper driver
 - Nema 17 or similar stepper motor
 - various leds, caps, and resistors

### Hardware:  
coming soon!


## Software

Code is based on my [PlantWaterBot](https://github.com/brianssparetime/PlantWaterBot), using a state machine and separate files for each
sensor/device to keep things organized and extendable.

