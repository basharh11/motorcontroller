#include "menu.h"
#include "ssd1309.h"
#include "queue.h"
#include "keypad.h"
#include <stdbool.h>

#define MAX_LENGTH 10

typedef enum {
    metric, imperial
} systemOfMeasurement; 

typedef enum {
    disabled, enabled
} state;

typedef enum {
    left, right
} direction;

extern char target[MAX_LENGTH];
extern char slowZone[MAX_LENGTH];
extern char motor1Range[MAX_LENGTH];
extern char motor2Range[MAX_LENGTH];
extern char motor1PeakSpeed[MAX_LENGTH];
extern char motor1Acceleration[MAX_LENGTH];
extern char motor1Pulse[MAX_LENGTH];
extern char motor2PeakSpeed[MAX_LENGTH];
extern char motor2Acceleration[MAX_LENGTH];
extern char motor2Pulse[MAX_LENGTH];

extern queue keyQueue;
extern bool arrowFlag;
extern MenuNode *current;

void navigationInit();
void navigationLoop();
void updateParameters();
