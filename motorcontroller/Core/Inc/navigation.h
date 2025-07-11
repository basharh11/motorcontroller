#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "menu.h"
#include "ssd1309.h"
#include "queue.h"
#include "keypad.h"
#include "parameters.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "motor.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern queue keyQueue;
extern bool arrowFlag;
extern menuNode *current;
extern struct motor m1;
extern struct movementProfile mp1;

void navigationInit();
void navigationLoop();
void updateParameters();
uint8_t selectInputScreen();
bool isInputScreen();

void dtoa(char *buf, float val, int precision);

void clearAll();

#endif

