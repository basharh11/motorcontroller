#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_gpio.h"

uint8_t readKeypad();

static const uint8_t keymap[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

static const uint32_t rowPins[4] = {
  LL_GPIO_PIN_7,  // row 0 (bottom)
  LL_GPIO_PIN_6,  // row 1
  LL_GPIO_PIN_5,  // row 2
  LL_GPIO_PIN_4   // row 3 (top)
};

static const uint32_t columnPins[4] = {
  LL_GPIO_PIN_3,  // col 0 (right)
  LL_GPIO_PIN_2,  // col 1
  LL_GPIO_PIN_1,  // col 2
  LL_GPIO_PIN_0   // col 3 (left)
};