#include "eeprom.h"

void write(uint16_t memoryAddress, uint8_t *wData, uint16_t size) {
    HAL_I2C_Mem_Write(&EEPROM_I2C_PORT, EEPROM_I2C_ADDR, memoryAddress, I2C_MEMADD_SIZE_16BIT, wData, size, HAL_MAX_DELAY);
    while(HAL_I2C_IsDeviceReady(&EEPROM_I2C_PORT, EEPROM_I2C_ADDR, 2, 5));
}

void shutdownTIMHandler(TIM_HandleTypeDef *htim) {
    if(htim->Instance != TIM4)
        return;
    
    shutdownCall();
}

void read(uint16_t memoryAddress, uint8_t *rData, uint16_t size) {
    HAL_I2C_Mem_Read(&EEPROM_I2C_PORT, EEPROM_I2C_ADDR, memoryAddress, I2C_MEMADD_SIZE_16BIT, rData, size, HAL_MAX_DELAY);
    while(HAL_I2C_IsDeviceReady(&EEPROM_I2C_PORT, EEPROM_I2C_ADDR, 2, 5));
}


void shutdownCall() {
    HAL_ADC_Start_IT(&hadc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *htim) {
    if(htim->Instance != ADC1)
        return;

    uint32_t adcValue = HAL_ADC_GetValue(htim);

    if(adcValue < 3000)
        powerDownRequested = true;
}