/*
 * File: SSD1309.h
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

// I2C handle defined & initialized in main.c
extern I2C_HandleTypeDef hi2c1;

// Display parameters
#define SSD1309_FB_WIDTH 128
#define SSD1309_FB_HEIGHT 64
#define SSD1309_FB_SIZE ((SSD1309_FB_WIDTH * SSD1309_FB_HEIGHT) / 8)
#define SSD1309_I2C_ADDR (0x3C << 1)

// SSD1309 Command Definitions
#define SSD1309_CMD_SET_CONTRAST_CONTROL             0x81
#define SSD1309_CMD_ENTIRE_DISPLAY_ON_RESUME         0xA4
#define SSD1309_CMD_ENTIRE_DISPLAY_ON                0xA5
#define SSD1309_CMD_NORMAL_DISPLAY                   0xA6
#define SSD1309_CMD_INVERSE_DISPLAY                  0xA7
#define SSD1309_CMD_DISPLAY_OFF                      0xAE
#define SSD1309_CMD_DISPLAY_ON                       0xAF
#define SSD1309_CMD_SET_MEMORY_ADDR_MODE             0x20
#define SSD1309_CMD_SET_COLUMN_ADDR                  0x21
#define SSD1309_CMD_SET_PAGE_ADDR                    0x22
#define SSD1309_CMD_SET_LOWER_COLUMN_START           0x00
#define SSD1309_CMD_SET_HIGHER_COLUMN_START          0x10
#define SSD1309_CMD_SET_PAGE_START_ADDR_BASE         0xB0
#define SSD1309_CMD_SET_DISPLAY_START_LINE           0x40
#define SSD1309_CMD_SET_SEGMENT_REMAP_NORMAL         0xA0
#define SSD1309_CMD_SET_SEGMENT_REMAP_REVERSE        0xA1
#define SSD1309_CMD_SET_COM_SCAN_DIR_NORMAL          0xC0
#define SSD1309_CMD_SET_COM_SCAN_DIR_REMAP           0xC8
#define SSD1309_CMD_SET_MULTIPLEX_RATIO              0xA8
#define SSD1309_CMD_SET_DISPLAY_OFFSET               0xD3
#define SSD1309_CMD_SET_COM_PINS                     0xDA
#define SSD1309_CMD_SET_PRECHARGE_PERIOD             0xD9
#define SSD1309_CMD_SET_VCOMH_DESELECT_LEVEL         0xDB
#define SSD1309_CMD_SET_DISPLAY_CLOCK_DIV            0xD5
#define SSD1309_CMD_SET_CHARGE_PUMP                  0x8D

// User API
void ssd1309_init(void);
void ssd1309_update(void);
void ssd1309_set_pixel(int x, int y, bool state);
bool ssd1309_get_pixel(int x, int y);
void ssd1309_draw_bitmap(int x, int y, int w, int h, const uint8_t *bitmap);
void ssd1309_draw_text(int x, int y, int size, const char *text);

#endif // SSD1309_H