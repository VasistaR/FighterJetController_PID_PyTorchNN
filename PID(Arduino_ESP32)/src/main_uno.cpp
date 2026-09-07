#include "controller.h"
#include <Arduino.h>

#define USETESTSETPOINT 0

Controller ctrl;
float testSetpoint = 0.75;
float downrange = 2000;
float height = -500;
float reading = 0.0;

void setup() {
    //set controller parameters
    ctrl.setPIDParameters(6,1,3);
    ctrl.setLimits(-1.57, 1.57);
    //set initial setpoint
    if (USETESTSETPOINT){
        ctrl.updateSetpoint(testSetpoint);
    } else {
        ctrl.updateSetpoint(downrange, height);
    }
    //start serial communication with system
    Serial.begin(9600);  //same as system for baud rate
}

//loop takes output of sensor, updates reading, updates error, assigns the input, prints error back
void loop() {

    //checks if data is available to read from system
    if (Serial.available() > 0) {
        reading = Serial.parseFloat();   
        ctrl.updateReading(reading);
        ctrl.updateError();
        //actual controller
        ctrl.calculateInput();            
        Serial.print("U_CMD,");
        Serial.println(ctrl.returnInput(), 7); 
    }
}