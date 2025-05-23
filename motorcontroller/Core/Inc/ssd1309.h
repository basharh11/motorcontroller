/*
 * File: ssd1309.h
 * Description: SSD1309 OLED Display I2C Driver Header
 * Author: Bashar Hamade
 * Created: 20 May 2025
 */

#ifndef SSD1309_H
#define SSD1309_H

#include "../../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "fonts.h"

// I2C Parameters
#define SSD1309_I2C_ADDR (0x3C << 1)
#define SSD1309_I2C_PORT hi2c1

// I2C handle defined & initialized in main.c
extern I2C_HandleTypeDef SSD1309_I2C_PORT;

// Display Parameters
#define SSD1309_HEIGHT 64
#define SSD1309_WIDTH 128
#define SSD1309_SIZE ((SSD1309_HEIGHT * SSD1309_WIDTH) >> 3)

// I2C API
void SSD1309_reset(void);
void SSD1309_init(void);
void SSD1309_update(void);

// User API
void SSD1309_setPixel(uint8_t x, uint8_t y, bool state);
bool SSD1309_getPixel(uint8_t x, uint8_t y);
void SSD1309_drawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
void SSD1309_drawText(uint8_t x, uint8_t y, uint16_t size, const char *text);

#endif // SSD1309_H