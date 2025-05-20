#include "../Inc/SSD1309.h"

uint8_t D_FRAMEBUFFER[D_FB_SIZE] = {0};

void d_set_pixel(int x, int y, bool state) {
    int pos = (D_FB_WIDTH * y + x) / 8; // formula to find position of pixel in continuous one-dimensional array; divide by 8 to convert from bytes to bits

    // set or clear the pixel
    // mask x with 0x7 to ensure the shift to 0x80 doesn't go out of bounds
    if(state) {
        D_FRAMEBUFFER[pos] |= (0x80 >> (x & 0x7)); 
    } else {
        D_FRAMEBUFFER[pos] &= ~(0x80 >> (x & 0x7));  
    }
 
}

bool d_get_pixel(int x, int y, bool state) {
    int pos = (D_FB_WIDTH * y + x) / 8;

    return D_FRAMEBUFFER[pos] & (0x80 >> (x & 0x7)); 
}

void d_draw_bitmap(int x, int y, int width, int height, const uint8_t *bitmap) {
    uint8_t checkMask = 0;
    int size = width * height / 8;

    int originalX = x;
    int originalY = y;

    int i, j;

    for(i = 0; i < size; i++) {
        for(j = 0; j < 8; j++) {
            checkMask = 0x80 >> j;

            d_set_pixel(x, y, bitmap[i] & checkMask);
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

void d_draw_text(int x, int y, int size, char* text) {
    const uint8_t *font;
    const uint8_t *drawChar;

    int runningX = x;
    int runningY = y;

    int target;
    int i;

    for(i = 0; i < strlen(text); i++) {
        if(runningX >= D_FB_WIDTH) {
            runningX = x;
            runningY += size;
        }

        if(runningY >= D_FB_HEIGHT) {
            return;
        }
        
        target = (size * size / 8) * (text[i] - ' '); // subtract the ASCII value of the spacebar for an offset
        drawChar = font + target;

        d_draw_bitmap(runningX, runningY, size, size, drawChar);
        
        runningX += size;
    }
}