/*
 * 
 * SSD1309 OLED Display I2C Driver
 * 
 * Author: Bashar Hamade
 * Created: 19 May 2025
 * 
 */

#ifndef SSD1309_H
#define SSD1309_H

#include "../../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h" // Needed for I2C
#include <stdint.h> 
#include <stdbool.h>

/*
 * DEFINES 
 */

#define D_FB_WIDTH 128
#define D_FB_HEIGHT 64
#define D_FB_SIZE (D_FB_WIDTH * D_FB_HEIGHT) / 8 // divide by 8 to convert from bytes to bits
#define SSD1309_I2C_ADDRESS (0x3C << 1) // shift left by 1 to make room for R/W bit

// SSD1309_CMD_CONTRAST_CONTROL (81h): Double-byte contrast control
#define SSD1309_CMD_CONTRAST_CONTROL             0x81
// SSD1309_CMD_ENTIRE_DISPLAY_ON_RESUME (A4h): Resume to RAM content display
#define SSD1309_CMD_ENTIRE_DISPLAY_ON_RESUME     0xA4
// SSD1309_CMD_ENTIRE_DISPLAY_ON (A5h): Force entire display ON, ignore RAM
#define SSD1309_CMD_ENTIRE_DISPLAY_ON            0xA5
// SSD1309_CMD_NORMAL_DISPLAY (A6h): Normal display (RAM 1 = pixel ON)
#define SSD1309_CMD_NORMAL_DISPLAY               0xA6
// SSD1309_CMD_INVERSE_DISPLAY (A7h): Inverse display (RAM 0 = pixel ON)
#define SSD1309_CMD_INVERSE_DISPLAY              0xA7
// SSD1309_CMD_DISPLAY_OFF (AEh): Display OFF (sleep mode)
#define SSD1309_CMD_DISPLAY_OFF                  0xAE
// SSD1309_CMD_DISPLAY_ON (AFh): Display ON (normal mode)
#define SSD1309_CMD_DISPLAY_ON                   0xAF
// SSD1309_CMD_NOP (E3h): No operation
#define SSD1309_CMD_NOP                          0xE3
// SSD1309_CMD_SET_COMMAND_LOCK (FDh): Lock/unlock command interface
#define SSD1309_CMD_SET_COMMAND_LOCK             0xFD
// SSD1309_CMD_RIGHT_HORIZ_SCROLL (26h): Right horizontal scroll setup
#define SSD1309_CMD_RIGHT_HORIZ_SCROLL           0x26
// SSD1309_CMD_LEFT_HORIZ_SCROLL (27h): Left horizontal scroll setup
#define SSD1309_CMD_LEFT_HORIZ_SCROLL            0x27
// SSD1309_CMD_CONT_V_AND_RH_SCROLL (29h): Continuous vertical + right horizontal scroll
#define SSD1309_CMD_CONT_V_AND_RH_SCROLL         0x29
// SSD1309_CMD_CONT_V_AND_LH_SCROLL (2Ah): Continuous vertical + left horizontal scroll
#define SSD1309_CMD_CONT_V_AND_LH_SCROLL         0x2A
// SSD1309_CMD_DEACTIVATE_SCROLL (2Eh): Deactivate scroll
#define SSD1309_CMD_DEACTIVATE_SCROLL            0x2E
// SSD1309_CMD_ACTIVATE_SCROLL (2Fh): Activate scroll
#define SSD1309_CMD_ACTIVATE_SCROLL              0x2F
// SSD1309_CMD_SET_LOWER_COLUMN_START (00h): Lower column start address
#define SSD1309_CMD_SET_LOWER_COLUMN_START       0x00
// SSD1309_CMD_SET_HIGHER_COLUMN_START (10h): Higher column start address
#define SSD1309_CMD_SET_HIGHER_COLUMN_START      0x10
// SSD1309_CMD_SET_MEMORY_ADDR_MODE (20h): Memory addressing mode
#define SSD1309_CMD_SET_MEMORY_ADDR_MODE         0x20
// SSD1309_CMD_SET_COLUMN_ADDR (21h): Column address set
#define SSD1309_CMD_SET_COLUMN_ADDR              0x21
// SSD1309_CMD_SET_PAGE_ADDR (22h): Page address set
#define SSD1309_CMD_SET_PAGE_ADDR                0x22
// SSD1309_CMD_SET_PAGE_START_ADDR_BASE (B0h): Page start address for Page mode
#define SSD1309_CMD_SET_PAGE_START_ADDR_BASE     0xB0
// SSD1309_CMD_SET_DISPLAY_START_LINE (40h): Display start line
#define SSD1309_CMD_SET_DISPLAY_START_LINE       0x40
// SSD1309_CMD_SET_SEGMENT_REMAP_NORMAL (A0h): Segment remap normal
#define SSD1309_CMD_SET_SEGMENT_REMAP_NORMAL     0xA0
// SSD1309_CMD_SET_SEGMENT_REMAP_REVERSE (A1h): Segment remap reverse
#define SSD1309_CMD_SET_SEGMENT_REMAP_REVERSE    0xA1
// SSD1309_CMD_SET_MULTIPLEX_RATIO (A8h): Multiplex ratio
#define SSD1309_CMD_SET_MULTIPLEX_RATIO          0xA8
// SSD1309_CMD_SET_COM_SCAN_DIR_NORMAL (C0h): COM output scan direction normal
#define SSD1309_CMD_SET_COM_SCAN_DIR_NORMAL      0xC0
// SSD1309_CMD_SET_COM_SCAN_DIR_REMAP (C8h): COM output scan direction remapped
#define SSD1309_CMD_SET_COM_SCAN_DIR_REMAP       0xC8
// SSD1309_CMD_SET_DISPLAY_OFFSET (D3h): Display offset
#define SSD1309_CMD_SET_DISPLAY_OFFSET           0xD3
// SSD1309_CMD_SET_COM_PINS (DAh): COM pins hardware configuration
#define SSD1309_CMD_SET_COM_PINS                 0xDA
// SSD1309_CMD_SET_GPIO (DCh): GPIO state
#define SSD1309_CMD_SET_GPIO                     0xDC
// SSD1309_CMD_SET_DISPLAY_CLOCK_DIV (D5h): Display clock divide ratio / oscillator frequency
#define SSD1309_CMD_SET_DISPLAY_CLOCK_DIV        0xD5
// SSD1309_CMD_SET_PRECHARGE_PERIOD (D9h): Pre-charge period
#define SSD1309_CMD_SET_PRECHARGE_PERIOD         0xD9
// SSD1309_CMD_SET_VCOMH_DESELECT_LEVEL (DBh): VCOMH deselect level
#define SSD1309_CMD_SET_VCOMH_DESELECT_LEVEL     0xDB
// SSD1309_CMD_SET_CHARGE_PUMP (8Dh): Charge pump setting
#define SSD1309_CMD_SET_CHARGE_PUMP              0x8D

#endif