#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ssd1309.h"
#include "queue.h"
#include "keypad.h"
#include "menu.h"
#include "motor.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void navigationInit();
void navigationLoop();
void updateParameters();
uint8_t selectInputScreen();
bool isInputScreen();

void dtoa(char *buf, float val, int precision);

#endif

