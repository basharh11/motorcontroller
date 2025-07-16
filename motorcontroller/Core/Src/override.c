#include "override.h"

// this overrides the weak HAL default for TIM
// when TIM2 ticks, HAL_TIM_IRQHandler(&htim2) is called by the NVIC which then calls this function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if(htim->Instance == TIM2) 
    keypadTIMHandler(htim); // send to keypad's row scanning handler
  if(htim->Instance == TIM4) 
    shutdownTIMHandler(htim); // send to keypad's row scanning handler
}

// this overrides the weak HAL default for GPIO EXTI
// when a column pin transitions to low, EXTI4_IRQHandler or EXTI9_5_IRQHandler calls this function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(GPIO_Pin != GPIO_PIN_0)
    keypadEXTIHandler(GPIO_Pin); // send to keypad's column handler
  else
    switchEXTIHandler(GPIO_Pin);
}

void HAL_PWR_PVD_Callback() {
  shutdownRoutine();
}

