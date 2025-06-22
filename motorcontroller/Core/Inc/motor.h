#include "navigation.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim3;

extern bool arrowDir;
extern bool home1;
extern bool home2;
extern bool emergencyStop;
extern bool relay1;
extern bool relay2;
extern bool analog;
extern bool units;
extern bool lastUnits;

void on_menu_move(void);
void motor_init_timer(void);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);