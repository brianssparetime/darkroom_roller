#include "Arduino.h" 
#include <AccelStepper.h>

// when uncommented, serial debug output is enabled; 
//    se sprintf stringbufs instead of String()
#define DEBUG



// stepper
#define ST_DIR A0
#define ST_STEP  10
#define ST_SLP 5


AccelStepper stepper(1, ST_STEP, ST_DIR);


void setup() {
    
    #ifdef DEBUG 
      Serial.begin(115200);
      Serial.println("online");
    #endif

    pinMode(ST_SLP, OUTPUT);
    stepper.setMaxSpeed(800);
   stepper.setAcceleration(200);

    #ifdef DEBUG 
      Serial.println("finished setup");
    #endif 
}



  /*     ----------    L O O P     --------- */





void loop() {

    stepper.runSpeed();

} // end loop
