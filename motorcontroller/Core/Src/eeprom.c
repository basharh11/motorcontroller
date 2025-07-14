#include "eeprom.h"


void write(uint16_t memoryAddress, uint8_t *wData) {
    HAL_I2C_Mem_Write(&EEPROM_I2C_PORT, EEPROM_I2C_ADDR, memoryAddress, I2C_MEMADD_SIZE_16BIT, wData, sizeof(&wData), HAL_MAX_DELAY);
}

void read(uint16_t memoryAddress, uint8_t *rData) {
    HAL_I2C_Mem_Read(&EEPROM_I2C_PORT, EEPROM_I2C_ADDR, memoryAddress, I2C_MEMADD_SIZE_16BIT, rData, sizeof(&rData), HAL_MAX_DELAY);
}