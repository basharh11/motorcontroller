#include "../Inc/keypad.h"

uint8_t readKeypad(void) {
    uint8_t row, col, key;

    // set all rows high before starting
    for (row = 0; row < 4; row++)
        LL_GPIO_SetOutputPin(GPIOC, rowPins[row]);

    for (row = 0; row < 4; row++) {
        // clear current row
        LL_GPIO_ResetOutputPin(GPIOC, rowPins[row]);
       
        HAL_Delay(1);

        // read each row
        for (col = 0; col < 4; col++) {
            HAL_Delay(10); // debounce delay
            
            // check if key is still pressed
            if (LL_GPIO_IsInputPinSet(GPIOC, columnPins[col]) == 0) {
                key = row * 4 + col;
                while(LL_GPIO_IsInputPinSet(GPIOC, columnPins[col]) == 0); // wait until release
                HAL_Delay(5);  // release debounce delay
                return key;
    }
        }

        // set current row before moving to the next
        LL_GPIO_SetOutputPin(GPIOC, rowPins[row]);
    }

    // no key pressed
    return 0xFF;
}