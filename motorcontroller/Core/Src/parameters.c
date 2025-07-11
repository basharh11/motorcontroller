#include "parameters.h"

void parametersInit(parameters *p) {
    memset(p, 0, sizeof *p);

    char *bufs[] = {p->target, p->slowZone, p->motor1Range, p->motor2Range, p->motor1PeakSpeed, p->motor1Acceleration, p->motor1Pulse, p->motor2PeakSpeed, p->motor2Acceleration, p->motor2Pulse, p->motor1Position, p->motor2Position};
   
    for (int i = 0; i < NUM_OF_PARAMETERS; ++i)
        p->strings[i] = bufs[i];

    p->arrowDir = right;

    p->numberLength = 9;
}

void updateParameters(parameters *p) {
    p->home1 = (menu21.child == &disabledCheck) ? dis : en;
    p->home2 = (menu22.child == &disabledCheck) ? dis : en;
    p->emergencyStop = (menu24.child == &disabledCheck) ? dis : en;
    p->relay1 = (menu33.child == &disabledCheck) ? dis : en;
    p->relay2 = (menu34.child == &disabledCheck) ? dis : en;
    p->analog = (menu4.child == &disabledCheck) ? dis : en;
    p->units = (menu5.child == &menu51) ? metric : imperial;
    p->numberLength = p->units ? 8 : 9;
}

state getState(const menuNode *m, const parameters *p) {
    menuNode *parent = getParent(m);
    if(parent == &menu21) 
        return p->home1;
    else if(parent == &menu22) 
        return p->home2;
    else if(parent == &menu24) 
        return p->emergencyStop;
    else if(parent == &menu33) 
        return p->relay1;
    else if(parent == &menu34) 
        return p->relay2;
    else if(parent == &menu4) 
        return p->analog;
}

systemOfMeasurement getUnits(const parameters *p) {
    return p->units;
}

uint8_t getNumberLength(const parameters *p) {
    return p->numberLength;
}



