#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "menu.h"
#include "ssd1309.h"
#include "queue.h"
#include "keypad.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <motor.h>

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
extern char *parameters[];
extern char slowZone[MAX_LENGTH];
extern char motor1Range[MAX_LENGTH];
extern char motor2Range[MAX_LENGTH];
extern char motor1PeakSpeed[MAX_LENGTH];
extern char motor1Acceleration[MAX_LENGTH];
extern char motor1Pulse[MAX_LENGTH];
extern char motor2PeakSpeed[MAX_LENGTH];
extern char motor2Acceleration[MAX_LENGTH];
extern char motor2Pulse[MAX_LENGTH];
extern char motor1Position[MAX_LENGTH];
extern char motor2Position[MAX_LENGTH];

extern TIM_HandleTypeDef htim3;

extern float motor1Pos;
extern float motor2Pos;

extern volatile int32_t motor1_step_count;
extern float motor1_steps_per_unit;

extern bool homing_reverse_started;
extern bool homing_active;

extern queue keyQueue;
extern bool arrowFlag;
extern MenuNode *current;

void navigationInit();
void navigationLoop();
void updateParameters();
uint8_t selectInputScreen();
bool isInputScreen();

void dtoa(char *buf, float val, int precision);

void clearAll();

#endif

