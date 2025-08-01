#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string.h>
#include "enums.h"
#include "menu.h"
#include "eeprom.h"

#define MAX_LENGTH 10
#define NUM_OF_PARAMETERS 11

extern volatile bool powerDownRequested;

typedef struct menuNode menuNode;

typedef struct parameters {
    char target[MAX_LENGTH];

    char slowZone[MAX_LENGTH];
    char motor1Range[MAX_LENGTH];
    char motor2Range[MAX_LENGTH];
    char motor1PeakSpeed[MAX_LENGTH];
    char motor1Acceleration[MAX_LENGTH];
    char motor1Pulse[MAX_LENGTH];
    char motor2PeakSpeed[MAX_LENGTH];
    char motor2Acceleration[MAX_LENGTH];
    char motor2Pulse[MAX_LENGTH];
    char motor1Position[MAX_LENGTH];
    char motor2Position[MAX_LENGTH];

    float motor1Pos;
    float motor2Pos;   

    bool arrowDir;
    bool home1;
    bool home2;
    bool emergencyStop;
    bool relay1;
    bool relay2;
    bool analog;
    bool units;
    bool lastUnits;

    uint8_t numberLength;
} parameters;

void parametersInit(parameters *p);

void updateParameters(parameters *p);
void updateAbilityLinkage(parameters *p);
void saveParameters(parameters *p);
void loadParameters(parameters *p);
void clearParameters(parameters *p);

#endif 
