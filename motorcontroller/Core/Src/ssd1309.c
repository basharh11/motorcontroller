/*
 * File: ssd1309.c
 * Description: SSD1309 OLED Display I2C Driver Implementation
 * Author: Bashar Hamade
 * Created: 20 May 2025
 */

#include "ssd1309.h"

static uint8_t SSD1309_FRAMEBUFFER[SSD1309_SIZE];

void SSD1309_reset(void) {} // for I2C do nothing

// Send a byte to the command register
void SSD1309_sendCommand(uint8_t cmd) {
    HAL_I2C_Mem_Write(&SSD1309_I2C_PORT, SSD1309_I2C_ADDR, 0x00, 1, &cmd, 1, HAL_MAX_DELAY);
}

// Send data
void SSD1309_sendData(uint8_t* data, size_t length) {
    HAL_I2C_Mem_Write(&SSD1309_I2C_PORT, SSD1309_I2C_ADDR, 0x40, 1, data, length, HAL_MAX_DELAY);
}

// Initialization procedure according to the SSD1309 display according to a datasheet provided by Solomon Systech Limited
// Using horizontal addressing mode
void SSD1309_init(void) {
    SSD1309_reset();

    HAL_Delay(100); // Wait for the screen to boot

    SSD1309_sendCommand(0xAF);

    SSD1309_sendCommand(0xA6);

    SSD1309_sendCommand(0x81); 
    SSD1309_sendCommand(0xFF); 
    
    SSD1309_sendCommand(0x20);
    SSD1309_sendCommand(0x00);

    SSD1309_sendCommand(0x21);
    SSD1309_sendCommand(0x00);
    SSD1309_sendCommand(SSD1309_WIDTH - 1);

    SSD1309_sendCommand(0x22);
    SSD1309_sendCommand(0x00);
    SSD1309_sendCommand((SSD1309_HEIGHT >> 3) - 1);

    SSD1309_sendCommand(0x40);
        
    SSD1309_sendCommand(0xA1);

    SSD1309_sendCommand(0xA8);
    SSD1309_sendCommand(0x3F);

    SSD1309_sendCommand(0xC8);

    SSD1309_sendCommand(0xD3);
    SSD1309_sendCommand(0x00);

    SSD1309_sendCommand(0xDA);
    SSD1309_sendCommand(0x12);

    SSD1309_sendCommand(0xD5);
    SSD1309_sendCommand(0xF0);

    SSD1309_sendCommand(0xD9);
    SSD1309_sendCommand(0x34);

    SSD1309_sendCommand(0xA4);

    memset(SSD1309_FRAMEBUFFER, 0, SSD1309_SIZE); // clear all bytes

    SSD1309_update();
}

// Push framebuffer to SSD1309
void SSD1309_update(void) {
    SSD1309_sendData(SSD1309_FRAMEBUFFER, SSD1309_SIZE);
}

void SSD1309_setPixel(uint8_t x, uint8_t y, bool state) {
    if(x >= SSD1309_WIDTH || y >= SSD1309_HEIGHT) 
        return;

    // formula to find position of pixel
    // shift y by 3 because although there are 64 rows, 8 rows are addressed as a single page by the display
    // this must be done because each byte of data sent in the I2C packet is sent as 8 vertical pixels
    // shifting 3 to the right is the same as dividing by 8 when dealing with bits
    uint16_t pos = (SSD1309_WIDTH * (y >> 3) + x);
    
    // set or clear the pixel
    // the mask chooses which bits within the byte should be turned on
    if(state)
       SSD1309_FRAMEBUFFER[pos] |= 0x1 << (y & 0x7);
    else 
       SSD1309_FRAMEBUFFER[pos] &= ~(0x1 << (y & 0x7));  
}

bool SSD1309_getPixel(uint8_t x, uint8_t y) {
    if(x >= SSD1309_WIDTH || y >= SSD1309_HEIGHT) 
        return false;
        
    uint16_t pos = SSD1309_WIDTH * (y >> 3) + x;

    return (SSD1309_FRAMEBUFFER[pos] & (1 << (y & 0x7))) != 0; 
}

void SSD1309_drawBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap) {
    uint8_t checkMask = 1;
    uint16_t size = (width * height) >> 3;

    uint8_t originalX = x;
    uint8_t originalY = y;

    uint16_t i;
    uint8_t j;

    for(i = 0; i < size; i++) {
        for(j = 0; j < 8; j++) {
            checkMask = 0x80 >> j;

            SSD1309_setPixel(x, y, bitmap[i] &checkMask);
            x++;

            if((x - originalX) == width) {
                y++;
                x = originalX;
            }

            if((y - originalY) == height) {
                return;
            }
        }
    }
}

void SSD1309_drawText(uint8_t x, uint8_t y, uint16_t size, char *text) {
    const uint8_t *font, *drawChar;

    uint8_t runningX = x;
    uint8_t runningY = y;

    uint16_t target, i;

    font = CGA8;

    for(i = 0; i < strlen(text); i++) {
        if(runningX >= SSD1309_WIDTH) {
            runningX = x;
            runningY += size;
        }

        if(runningY >= SSD1309_HEIGHT)
            return;
        
        target = ((size * size) >> 3) * (text[i] - ' '); // subtract the ASCII value of the spacebar for an offset
        drawChar = font + target;

        SSD1309_drawBitmap(runningX, runningY, size, size, drawChar);
        
        if(text[i] == '.' && text[i+1] == '1')
            runningX += 4;
        else if(text[i+1] == '1') 
            runningX += 6;
        else if(text[i] == '.') 
            runningX += 5;
        else {
            runningX += 7;
        }
    }
}
