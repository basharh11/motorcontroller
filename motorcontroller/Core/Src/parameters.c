#include "parameters.h"

void parametersInit(parameters *p) {
    memset(p, 0, sizeof(*p));
    
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
    if(p->units != p->lastUnits)
        clearParameters(p);
    p->lastUnits = p->units;
}

void saveParameters(parameters *p) {
    if(powerDownRequested) {
        powerDownRequested = false;
        writeBlock(0x00, (uint8_t*)p, sizeof(*p));  
        HAL_TIM_Base_Stop_IT(&htim4);
    }  
}

void loadParameters(parameters *p) {
    read(0x00, (uint8_t*)p, sizeof(*p));
}

void clearParameters(parameters *p) {
    p->target[0] = '\0';
    p->motor1Range[0] = '\0';
    p->motor2Range[0] = '\0';
    p->motor1PeakSpeed[0] = '\0';
    p->motor1Acceleration[0] = '\0';
    p->motor1Pulse[0] = '\0';
    p->motor2PeakSpeed[0] = '\0';
    p->motor2Acceleration[0] = '\0';
    p->motor2Pulse[0] = '\0';
}





