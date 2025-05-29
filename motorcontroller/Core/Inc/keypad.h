#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_gpio.h"

void initKeypad(void);
uint8_t readKeypad(void);

static const uint8_t keymap[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

static const uint32_t rowPins[4] = {
  LL_GPIO_PIN_0,  // row 0 (top)
  LL_GPIO_PIN_1,  // row 1
  LL_GPIO_PIN_2,  // row 2
  LL_GPIO_PIN_3   // row 3 (bottom)
};

static const uint32_t columnPins[4] = {
  LL_GPIO_PIN_4,  // col 0 (left)
  LL_GPIO_PIN_5,  // col 1
  LL_GPIO_PIN_6,  // col 2
  LL_GPIO_PIN_7   // col 3 (right)
};

