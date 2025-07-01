#ifndef MOTOR_H
#define MOTOR_H

#include "navigation.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim3;

extern bool arrowDir;
extern bool home1;
extern bool home2;
extern bool emergencyStop;
extern bool relay1;
extern bool relay2;
extern bool analog;
extern bool units;
extern bool lastUnits;

extern char motor1Pulse[MAX_LENGTH];
extern volatile int32_t motor1_step_count;
extern float motor1_steps_per_unit;
extern float motor1Pos;

void on_menu_move(void);
void motor_init_timer(void);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
void home(void);

#endif