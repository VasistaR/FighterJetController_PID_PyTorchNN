#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "math.h"

//more defining of methods
class Controller{
    public:
        Controller();
        void updateReading(const float& val); //updayte sensor reading
        void calculateInput();
        void updateSetpoint(const float& val);
        void updateSetpoint(const float& dist, const float& height);
        void updateError(); //setpoint-output
        float& returnInput(); //send to serial port
        void setPIDParameters(const float _kp, const float _ki, const float _kd);
        void setLimits(const float lower, const float upper); //set saturation limits
        void overrideError();

    private:
        const float h; //timestep
        float _calculatedInput;
        float _error;
        float _prev_error; //previous time step error
        float _reading; //sensor reading
        float _tot_error; //total error
        float _setpoint;
        float KP;
        float KI;
        float KD;
        float _sat_lower;
        float _sat_upper;
        bool initTrigger;
};


#endif