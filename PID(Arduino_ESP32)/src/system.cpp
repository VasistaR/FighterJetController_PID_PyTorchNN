#include "system.h"

//timestep defined here at 0.005
System::System():h(0.005){
    initialize();
}

void System::initialize(){
    //States X and inputs U at t = 0
    X[0] = 0;
    X[1] = 0;
    X[2] = 0;
    U[0] = 0.0;
}

//overall state equation done in steps
void System::simStep(){
    //A*X
    NX1_Matmul(_prodAX, _A, X);
    //B*U
    NX1_Matmul(_prodBU, B, U);
    //dXdT = A*X+BU
    NX1_Add(_dXdT, _prodAX, _prodBU);
    //dXdT = h*dXdT;
    //x = x + h*dXdT
    scaleArray(_dXdT, h);
    //X = X + dXdT
    NX1_Add(X, X, _dXdT);
    //Y = X[2]
    _Y = X[2]; //pitch angle
}


//assign value of U
void System::updateControllerCommand(const float& val){
    U[0] = val;
}


//return output
float& System::returnOutput(){
    return _Y;
}

