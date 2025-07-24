# Darkroom Print Processor

Instead of developing photographic prints in a series of trays for each chemical process, a 
rotary or drum processor uses a single light-tight drum for all processes.  This saves space,
especially as the print size scales, uses less chemistry, and preserves that chemistry for
longer (since it's not sitting open to the air).  Bonus, you're not smelling a tray of fixer.

I've been using Ilford Cibachrome drums, but the more expensive Jobo drums work too.

![Cibachrome drums in 8x10 and 11x14](img/Cibachrome%20drums.jpg)

This processor is open source, easy to assemble, and easy to use.  

All electronics sit on the top, so the base of the processor can easily be 
submerged in a tub and used with an independent sous-vide heater for temperature controlled 
color processing/developing.  

A stepper motor and rollerblade wheel drive the drum from above, on a hinged arm.  Speed is about 
60-75 rpm.  A tilt switch automatically detects drum size (4" or 6"), and adjusts rotation speed to 
match.  Different drum sizes and rotation speeds easily configurable in code.

The entire project can be assembled on a low budget (< $100), and probably a lot less if you 
have any this stuff lying around.


## Usage:
 - Turn processor on with on/off switch
 - Short press on the rotary encoder moves between setting digits of the timer (1x seconds, 10x
seconds, 1x minutes).
 - Rotating the encoder changes the value of the selected digit
 - Long press starts the rotary cycle
 - Any control input while running causes it to stop.



## Hardware (non-electronic):

 - [1.5 inch casters](https://www.amazon.com/gp/product/B09V74CMRQ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) mounted to a board.
 - For 8x10 and 11x14 Ilford drums, spacing between the casters is ___ and for the 16x20 Ilford drum or the Jobo drums, spacing is ___.
 - I mounted one row of the casters with magnets, so it can easily be moved and locked into position.

More generally, to solve for the distance between the bottom-most part of the drum and the surface, with:
 - R is your drum radius
 - c is your caster radius
 - h is the height of the caster from plate to center of the wheel
 - d is half the distance between the the casters
 - k is the clearance between the surface and the bottom-most point of your drum

then the distance from the centerline to the roller/caster centers (d) is equal to sqrt ( (r+c)^2 + (r+k-h)^2 )

![caster geometry](img/caster%20geometry.jpeg)

You could of course stop here and just rotate the drum by hand, but what fun is that?

## Additional Hardware (for electronic operation):

 - [Nema 17 motor mount](https://www.pololu.com/product/2266)
 - [84x24mm roller blade wheel](https://www.pololu.com/product/3275)
 - [Scooter wheel hub adapter](https://www.pololu.com/product/2673)
 - basic cabinet hinge

## Electronics:

The heart of the project is an old-school 5v Arduino Nano.

Connected to the Arduino via the custom pcb are:
 - Arduino Nano 5v (the old one with old usb-mini plugs)
 - TM1637 4 digit 7 segment display
 - KY-040 rotary encoder
 - A4988 (or DRV8825) stepper driver
 - Nema 17 stepper motor
 - LM7805 voltage regulator
 - one 470 uF filtering cap for the stepper driver
 - three 10k resistors for the rotary encoder


Some optional components you can choose to populate or not on the board are:
 - LED (and 330 ohm resistor), which blinks on completion
 - piezo beeper (and 330 ohm resistor), which beeps on completion
 - tilt sensor for detecting two different drum sizes
 - (4) 470nF capacitors, to help with debounce, although software debounce is pretty good
 - (currently unused) header for i2c (and 10k resistor), available for additional sensors and/or future expansion
 - (currently unused) 2 extra 3-pin header for D3 and D4 (each with vcc and ground pins), available for additional sensors and/or future expansion


All soldering is through-hole, and can be done with a simple soldering iron in about 15-20 minutes.

### power

The voltage regulator should work fine with wall wart power supplies in the 9-12v range.

The supply should also have sufficient amperage to run the stepper.  I'm using a ____.

Rather that mess around with connector sizes and polarity, just cut the end off your power cord and 
solder it onto the PCB.  Check polarity with a multimeter.

## Software

Code is based on my [PlantWaterBot](https://github.com/brianssparetime/PlantWaterBot), using a state machine and separate files for each
sensor/device to keep things organized, understandable, and extendable.


## Build your own?

Building the basic hardware is very straight forward, and really only requires a drill and screw driver.

Assembling the electronics only requires a soldering iron.

Download the code, compile, and upload to the Arduino, and you are in business.


