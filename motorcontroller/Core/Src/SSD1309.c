/*
 * File: SSD1309.c
 * Description: SSD1309 OLED Display I2C Driver Implementation
 * Author: Bashar Hamade
 * Created: 20 May 2025
 */

#include "../Inc/SSD1309.h"

void ssd1306_Reset(void) {
    /* for I2C - do nothing */
}

// Send a byte to the command register
void ssd1306_WriteCommand(uint8_t byte) {
    HAL_I2C_Mem_Write(&SSD1309_I2C_PORT, SSD1309_I2C_ADDR, 0x00, 1, &byte, 1, HAL_MAX_DELAY);
}

// Send data
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
    HAL_I2C_Mem_Write(&SSD1309_I2C_PORT, SSD1309_I2C_ADDR, 0x40, 1, buffer, buff_size, HAL_MAX_DELAY);
}


void ssd1309_init(void) {
    // —————————————————————————————————————————
    // 1) Wait for display power rails to stabilize
    //    (tAF ≈100 ms before SEG/COM turn-on) :contentReference[oaicite:0]{index=0}
    // —————————————————————————————————————————
    HAL_Delay(100);

    // —————————————————————————————————————————
    // 2) Display OFF (sleep mode) (AEh) :contentReference[oaicite:1]{index=1}
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_DISPLAY_OFF);

    // —————————————————————————————————————————
    // 3) Charge pump ON (8Dh, 14h) :contentReference[oaicite:2]{index=2}
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_SET_CHARGE_PUMP);
    SSD1309_SendCommand(0x14);

    // —————————————————————————————————————————
    // 4) Memory addressing: Horizontal mode (20h, 00h)  
    //    RESET default = 00h :contentReference[oaicite:3]{index=3}
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_SET_MEMORY_ADDR_MODE);
    SSD1309_SendCommand(0x00);

    // —————————————————————————————————————————
    // 5) Column & page address ranges (21h/22h)  
    //    RESET: 0 → 127, pages 0 → 7 :contentReference[oaicite:4]{index=4}
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_SET_COLUMN_ADDR);
    SSD1309_SendCommand(0x00);
    SSD1309_SendCommand(SSD1309_FB_WIDTH - 1);

    SSD1309_SendCommand(SSD1309_CMD_SET_PAGE_ADDR);
    SSD1309_SendCommand(0x00);
    SSD1309_SendCommand((SSD1309_FB_HEIGHT / 8) - 1);

    // —————————————————————————————————————————
    // 6) Segment remap & COM scan direction (A1h, C8h)  
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_SET_SEGMENT_REMAP_REVERSE);
    SSD1309_SendCommand(SSD1309_CMD_SET_COM_SCAN_DIR_REMAP);

    // —————————————————————————————————————————
    // 7) COM pins hardware configuration (DAh, 12h)  
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_SET_COM_PINS);
    SSD1309_SendCommand(0x12);

    // —————————————————————————————————————————
    // 8) Contrast (81h, 7Fh), Pre-charge (D9h, 22h), VCOMH (DBh, 20h)  
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_SET_CONTRAST_CONTROL);
    SSD1309_SendCommand(0x7F);

    SSD1309_SendCommand(SSD1309_CMD_SET_PRECHARGE_PERIOD);
    SSD1309_SendCommand(0x22);

    SSD1309_SendCommand(SSD1309_CMD_SET_VCOMH_DESELECT_LEVEL);
    SSD1309_SendCommand(0x20);

    // —————————————————————————————————————————
    // 9) Resume RAM content & normal display (A4h, A6h)  
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_ENTIRE_DISPLAY_ON_RESUME);
    SSD1309_SendCommand(SSD1309_CMD_NORMAL_DISPLAY);

    // —————————————————————————————————————————
    // 10) Display ON (AFh) :contentReference[oaicite:5]{index=5}
    // —————————————————————————————————————————
    SSD1309_SendCommand(SSD1309_CMD_DISPLAY_ON);

    // —————————————————————————————————————————
    // 11) Clear framebuffer & push to screen
    // —————————————————————————————————————————
    memset(SSD1309_FRAMEBUFFER, 0, SSD1309_FB_SIZE);
    ssd1309_update();
}


void ssd1309_update(void) {
    for (uint8_t page = 0; page < (SSD1309_FB_HEIGHT / 8); ++page) {
        SSD1309_SendCommand(SSD1309_CMD_SET_PAGE_ADDR | page);
        SSD1309_SendCommand(SSD1309_CMD_SET_LOWER_COLUMN_START);
        SSD1309_SendCommand(SSD1309_CMD_SET_HIGHER_COLUMN_START);
        SSD1309_SendData(&SSD1309_FRAMEBUFFER[page * SSD1309_FB_WIDTH], SSD1309_FB_WIDTH);
    }
}

void ssd1309_set_pixel(int x, int y, bool state) {
    int pos = (SSD1309_FB_WIDTH * y + x) / 8; // formula to find position of pixel in continuous one-dimensional array; divide by 8 to convert from bytes to bits

    // set or clear the pixel
    // mask x with 0x7 to ensure the shift to 0x80 doesn't go out of bounds
    if(state) {
       SSD1309_FRAMEBUFFER[pos] |= (0x80 >> (x & 0x7)); 
    } else {
       SSD1309_FRAMEBUFFER[pos] &= ~(0x80 >> (x & 0x7));  
    }
 
}

bool ssd1309_get_pixel(int x, int y) {
    int pos = (SSD1309_FB_WIDTH * y + x) / 8;

    return SSD1309_FRAMEBUFFER[pos] & (0x80 >> (x & 0x7)); 
}

void ssd1309_draw_bitmap(int x, int y, int width, int height, const uint8_t *bitmap) {
    uint8_t checkMask = 0;
    int size = width * height / 8;

    int originalX = x;
    int originalY = y;

    int i, j;

    for(i = 0; i < size; i++) {
        for(j = 0; j < 8; j++) {
            checkMask = 0x80 >> j;

            ssd1309_set_pixel(x, y, bitmap[i] & checkMask);
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

void ssd1309_draw_text(int x, int y, int size, const char *text) {
    const uint8_t *font;
    const uint8_t *drawChar;

    int runningX = x;
    int runningY = y;

    int target;
    int i;

    font = CGA8;

    for(i = 0; i < strlen(text); i++) {
        if(runningX >= SSD1309_FB_WIDTH) {
            runningX = x;
            runningY += size;
        }

        if(runningY >= SSD1309_FB_HEIGHT) {
            return;
        }
        
        target = (size * size / 8) * (text[i] - ' '); // subtract the ASCII value of the spacebar for an offset
        drawChar = font + target;

        ssd1309_draw_bitmap(runningX, runningY, size, size, drawChar);
        
        runningX += size;
    }
}

