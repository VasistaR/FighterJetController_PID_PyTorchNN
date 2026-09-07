#include <Arduino.h>
#include <system.h>

System sys;

float timestep = 0;
float maxTimestep = 3000;

//this sample code provided by www.programmingboss.com
void setup() {
    // Start serial communication with Arduino Uno
    Serial.begin(9600); //baud rate = 9600
    // Give some time to connect the Uno, open Python
    delay(20000);  //20 seconds
}

void loop() {
    
    if (timestep <= maxTimestep) {
        
        //step and send output to controller
        sys.simStep();
        float& y = sys.returnOutput();
        Serial.print("Y,"); //sends value to controller
        Serial.println(y, 7); //sends value to controller
        
        //reads input from controller
        if (Serial.available() > 0) {
            float u_cmd = Serial.parseFloat(); //cmd is control system input, updates value of U, affects system in next time step
            sys.updateControllerCommand(u_cmd);
        }

    } else{
        Serial.println("STOP");
        delay(5000);
    }
      
    timestep++;
    delay(100);
}