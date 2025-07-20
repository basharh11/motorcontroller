#ifndef EEPROM_H
#define EEPROM_H

#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define EEPROM_I2C_ADDR 0xA0
#define EEPROM_I2C_PORT hi2c2

extern I2C_HandleTypeDef EEPROM_I2C_PORT;

extern ADC_HandleTypeDef hadc1;

extern TIM_HandleTypeDef htim4;

extern volatile bool powerDownRequested;

void write(uint16_t memoryAddress, uint8_t *wData, uint16_t size);
void read(uint16_t memoryAddress, uint8_t *rData, uint16_t size);

void shutdownTIMHandler(TIM_HandleTypeDef *htim);

void shutdownCall();
void shutdownRoutine();

#endif