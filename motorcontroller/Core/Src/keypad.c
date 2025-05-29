#include "../Inc/keypad.h"

void initKeypad(void) {
    uint8_t row;

    // set all rows high
    for(row = 0; row < 4; row++)
        LL_GPIO_SetOutputPin(GPIOC, rowPins[row]);
}

// uint8_t readKeypad(void) {
//     uint8_t row, col;

//     for (row = 0; row < 4; row++) {
//         // clear current row
//         LL_GPIO_ResetOutputPin(GPIOC, rowPins[row]);

//         // read each row
//         for (col = 0; col < 4; col++) {            
//             if (LL_GPIO_IsInputPinSet(GPIOC, columnPins[col]) == 0)
//                 return row * 4 + col;
//         }

//         HAL_Delay(10);

//         LL_GPIO_SetOutputPin(GPIOC, rowPins[row]); // set current row before moving to the next   
//     }

//     // no key pressed
//     return 0xFF;
// }


uint8_t readKeypad(void) {
    uint8_t row, col;

    for(row = 0; row < 4; row++) {
        // clear current row
        LL_GPIO_ResetOutputPin(GPIOC, rowPins[row]);
        HAL_Delay(1);      

        for(col = 0; col < 4; col++) {
            // if column c reads low, key (r,c) is pressed
            if (!LL_GPIO_IsInputPinSet(GPIOC, columnPins[col])) {
                HAL_Delay(5);   // simple debounce
                if (!LL_GPIO_IsInputPinSet(GPIOC, columnPins[col])) {
                    LL_GPIO_SetOutputPin(GPIOC, rowPins[row]);
                    return keymap[row][col];
                }
            }
        }
        // no key in this row, restore it
        LL_GPIO_SetOutputPin(GPIOC, rowPins[row]);
    }
    return 0xFF;  // no key pressed
}