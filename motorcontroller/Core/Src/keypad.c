#include "keypad.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define KEYPAD_DEBOUNCE 200 // delay in ms to prevent rapid double clicking within the mouse

static uint8_t currentRow = 0;
static queue *keyQueue = NULL; // pointer to user's queue
static uint32_t previous = 0; // timestamp for debounce

// lookup table for decoding keys
static const char keymap[4][4] = {{ '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' }};

void keypadInit(queue *q) {
    keyQueue = q;
}

void keypadTIMHandler(TIM_HandleTypeDef *htim) {
    // early return if this callback wasn’t invoked by TIM2
    // htim->Instance holds which timer generated this interrupt
    if (htim->Instance != TIM2) 
        return;

    // set all rows high
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);

    // reset current row
    // since GPIO pins 0 to 3 are just multiples of two shifting GPIO_PIN_0 by the current row each time doubles it to the proper row
    HAL_GPIO_WritePin(GPIOC, (GPIO_PIN_0 << currentRow), GPIO_PIN_RESET);

    // increment current row
    // the 0x03 mask ensures that the interrupt effectively cycles through 0, 1, 2, and 3 as it continues to be called
    // this is because when it gets 3 the operation (3 + 1) & 0x03 = 4 & 0x03 = 0 will wrap it back around to 0

    currentRow = (currentRow + 1) & 0x03;
}

void keypadEXTIHandler(uint16_t GPIO_Pin) {
    if (!keyQueue) 
        return;  // must register queue first

    uint32_t now = HAL_GetTick(); // returns a millisecond counter incremented by SysTick

    // if the new interrupt is received within 200 ms of the previous one 
    // clear the pending interrupt bit on the corresponding GPIO pin and return to ensure it won't be processed
    if (now - previous < KEYPAD_DEBOUNCE) { 
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); 
        return;
    }

    previous = now; // update the previous timestamp to reflect the one corresponding to the current and valid button press

    uint8_t col;

    switch(GPIO_Pin) {
        case GPIO_PIN_4:
            col = 0; 
            break;
        case GPIO_PIN_5: 
            col = 1; 
            break;
        case GPIO_PIN_6: 
            col = 2; 
            break;
        case GPIO_PIN_7: 
            col = 3; 
            break;
        default:
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
            return;
    }

    uint8_t row = (currentRow + 3) & 0x03;
    uint8_t key = row * 4 + col + 1;

    enqueue(keyQueue, key);

    // clear pending interrupt to avoid retrigger
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}

char keypadDecodeKey(uint8_t key) {
    if (key < 1 || key > 16) 
        return 0;
    
    uint8_t row = (key - 1) / 4;
    uint8_t col = (key - 1) % 4;
    
    return keymap[row][col];
}
