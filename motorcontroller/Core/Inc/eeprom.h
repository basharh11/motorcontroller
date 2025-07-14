#ifndef EEPROM_H
#define EEPROM_H

#include "stm32f4xx_hal.h"

#define EEPROM_I2C_ADDR 0xA0
#define EEPROM_I2C_PORT hi2c2

extern I2C_HandleTypeDef EEPROM_I2C_PORT;

void write(uint16_t memoryAddress, uint8_t *wData);
void read(uint16_t memoryAddress, uint8_t *rData);

#endif