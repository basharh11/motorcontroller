    #include "motor.h"
    #include "main.h"
    #include <math.h>

    static uint32_t timerTickHz;
    static int32_t  pulses_left;
    static uint32_t total_pulses;
    static uint32_t step_index;
    static uint32_t accel_steps;
    static uint32_t cruise_steps;
    static uint32_t decel_steps;
    static float cruise_rate;
    static float accel_rate;

    volatile int32_t motor1_step_count = 0;
    float motor1_steps_per_unit;

    bool homing_active = false;
    bool homing_reverse_started = false;
    static uint32_t homing_dt_ticks;

    static uint32_t TimerClockHz(void) {
        uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
        if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) 
            pclk1 *= 2;
        return pclk1;
    }

    void motor_init_timer(void) {
        timerTickHz = TimerClockHz() / (htim3.Init.Prescaler + 1);
        HAL_TIM_Base_Start(&htim3);
    }

    void on_menu_move(void) {
        float ppu = strtof(motor1Pulse, NULL);
        motor1_steps_per_unit = ppu;
        float vmax_u_s = strtof(motor1PeakSpeed, NULL);
        float accel_u_s2 = strtof(motor1Acceleration, NULL);
        float dist_u = strtof(target, NULL);

        total_pulses = (uint32_t)roundf(dist_u * ppu);
        accel_rate = accel_u_s2 * ppu;
        float v_req = vmax_u_s * ppu;
        float v_peak = sqrtf(2.0f * accel_rate * total_pulses);
        cruise_rate = v_req < v_peak ? v_req : v_peak;

        float dp = (cruise_rate * cruise_rate) / (2.0f * accel_rate);
        if (dp >= total_pulses) {
            accel_steps  = total_pulses / 2;
            cruise_steps = 0;
        } else {
            accel_steps  = (uint32_t)ceilf(cruise_rate * cruise_rate / (2.0f * accel_rate));
            cruise_steps = total_pulses - 2 * accel_steps;
        }
        decel_steps = accel_steps;

        pulses_left   = total_pulses;
        step_index    = 0;
        homing_active = false;

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

        uint32_t dt0 = (uint32_t)(sqrtf(2.0f / accel_rate) * timerTickHz + 0.5f);
   
        __HAL_TIM_SET_AUTORELOAD(&htim3, dt0 - 1);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dt0 / 2);

        HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_3);
    }

   void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
        if (htim->Instance != TIM3) 
            return;

        if (homing_active) {
            if(homing_reverse_started) {
                float ppu = strtof(motor1Pulse, NULL);
                float vmax_u_s = 2;
                float homing_pps = vmax_u_s * ppu;
                if (homing_pps < 1.0f) 
                    homing_pps = 1.0f;
                uint32_t dt = (uint32_t)(timerTickHz / homing_pps + 0.5f);

                homing_dt_ticks = dt;
            }
            __HAL_TIM_SET_AUTORELOAD(&htim3, homing_dt_ticks - 1);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, homing_dt_ticks / 2);
            return;
        }

        if (--pulses_left <= 0) {
            HAL_TIM_OC_Stop_IT(&htim3, TIM_CHANNEL_3);
            return;
        }

        uint32_t dt_ticks;
        if (step_index < accel_steps) {
            float t0 = sqrtf(2.0f * step_index / accel_rate);
            float t1 = sqrtf(2.0f * (step_index+1) / accel_rate);
            dt_ticks = (uint32_t)((t1 - t0) * timerTickHz + 0.5f);
        } else if (step_index < accel_steps + cruise_steps) {
            dt_ticks = (uint32_t)(timerTickHz / cruise_rate + 0.5f);
        } else {
            uint32_t i = total_pulses - step_index;
            float t0 = sqrtf(2.0f * i / accel_rate);
            float t1 = sqrtf(2.0f * (i-1) / accel_rate);
            dt_ticks = (uint32_t)((t0 - t1) * timerTickHz + 0.5f);
        }

        __HAL_TIM_SET_AUTORELOAD(&htim3, dt_ticks - 1);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dt_ticks / 2);

        step_index++;

        motor1_step_count += (arrowDir ? -1 : +1);
        motor1Pos = motor1_step_count / motor1_steps_per_unit;
    }

    void switchEXTIHandler(uint16_t GPIO_Pin) {
        if(GPIO_Pin != GPIO_PIN_0)    
            return;  

        if(!homing_active)            
            return;  

        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET && !homing_reverse_started) {
            arrowDir = !arrowDir;
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);
            homing_reverse_started = true;
        }             
    }

    void home(void) {
        float ppu = strtof(motor1Pulse, NULL);
        motor1_steps_per_unit = ppu;
        float vmax_u_s = 10;

        float homing_pps = vmax_u_s * ppu;
        if (homing_pps < 1.0f) 
            homing_pps = 1.0f;

        uint32_t dt = (uint32_t)(timerTickHz / homing_pps + 0.5f);

        homing_dt_ticks = dt;
        homing_active = true;
        homing_reverse_started = false;

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

        __HAL_TIM_SET_AUTORELOAD(&htim3, dt - 1);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dt / 2);

        HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_3);
    }