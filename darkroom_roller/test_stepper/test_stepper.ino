#include "Arduino.h" 


// when uncommented, serial debug output is enabled; 
//    but this causes some instability (possible memory issues)!
//    technically, I should rewrite printlns to use sprintf stringbufs instead of String()
#define DEBUG



// stepper
#define ST_DIR A0
#define ST_STEP  10
#define ST_SLP 5



void setup() {
    
    #ifdef DEBUG 
      Serial.begin(115200);
      Serial.println("online");
    #endif

    AccelStepper stepper(1, ST_STEP, ST_DIR);
    pinMode(ST_SLP, OUTPUT);
    digitalWrite(ST_SLP, HIGH);
    stepper.setMaxSpeed(_max_speed);
    stepper.setSpeed(_max_speed);

    #ifdef DEBUG 
      Serial.println("finished setup");
    #endif 
}



  /*     ----------    L O O P     --------- */



void loop() {

    stepper.runSpeed();

} // end loop
