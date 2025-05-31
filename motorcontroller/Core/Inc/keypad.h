#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f4xx_hal.h"
#include "queue.h"
#include <stdint.h>

void keypadInit(queue *q);
void keypadTIMHandler(TIM_HandleTypeDef *htim);
void keypadEXTIHandler(uint16_t GPIO_Pin);
char keypadDecodeKey(uint8_t key);

#endif // KEYPAD_H