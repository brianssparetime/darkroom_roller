# Darkroom Print Processor

Instead of developing photographic prints in a series of trays for each chemical process, a 
rotary or drum processor uses a single light-tight drum for all processes.  This saves space,
especially as the print size scales, uses less chemistry, and preserves that chemistry for
longer (since it's not sitting open to the air).  Bonus, you're not smelling a tray of fixer.

I've been using Ilford Cibachrome drums, but the more expensive Jobo drums work too.

![Cibachrome drums in 8x10 and 11x14](img/Cibachrome%20drums.jpg)

This processor is open source, easy to assemble, and easy to use.  All electronics sit on the top,
so the base of the processor can easily be submerged in a tub and used with a sous-vide heater for
temperature controlled color processing/developing.

The entire project can be assembled on a low budget (< $100), and probably a lot less if you 
have any this stuff lying around.


## Usage:
 - Turn processor on with on/off switch
 - Short press on the rotary encoder moves between setting digits of the timer (1x seconds, 10x
seconds, 1x minutes).
 - Rotating the encoder changes the value of the selected digit
 - Long press starts the rotary cycle
 - Any control input while running causes it to stop.
 - Tilt switch automatically detects drum size, and adjusts rotation speed to match


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

 - TM1637 4 digit 7 segment display
 - KY-040 rotary encoder
 - A4988 or DRV8825 stepper driver
 - piezo buzzer
 - Nema 17 stepper motor
 - a few resistors and caps 

All soldering is through-hole, and can be done with a simple soldering iron in about 20 minutes.


## Software

Code is based on my [PlantWaterBot](https://github.com/brianssparetime/PlantWaterBot), using a state machine and separate files for each
sensor/device to keep things organized and extendable.


## Build your own?

Building the basic hardware is very straight forward, and really only requires a drill and screw driver.

Assembling the electronics only requires a soldering iron.

Download the code, compile, and upload to the Arduino, and you are in business.


