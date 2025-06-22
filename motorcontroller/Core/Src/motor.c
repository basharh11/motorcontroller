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
static float    cruise_rate;
static float    accel_rate;

static uint32_t TimerClockHz(void) {
    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) {
        pclk1 *= 2;
    }
    return pclk1;
}

void motor_init_timer(void) {
    timerTickHz = TimerClockHz() / (htim3.Init.Prescaler + 1);
}

void on_menu_move(void) {
    float ppu        = strtof(motor1Pulse, NULL);
    float vmax_u_s   = strtof(motor1PeakSpeed, NULL);
    float accel_u_s2 = strtof(motor1Acceleration, NULL);
    float dist_u     = strtof(target, NULL);

    if (arrowDir)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

    total_pulses = (uint32_t)roundf(dist_u * ppu);
    float v_cruise = fabsf(vmax_u_s   * ppu);
    float a        = fabsf(accel_u_s2 * ppu);

    float dp = (v_cruise * v_cruise) / (2.0f * a);
    if (dp >= total_pulses) {
        accel_steps = total_pulses / 2;
        cruise_rate = sqrtf(a * (float)total_pulses);
    } else {
        accel_steps = (uint32_t)ceilf(v_cruise * v_cruise / (2.0f * a));
        cruise_rate = v_cruise;
    }
    decel_steps = accel_steps;
    cruise_steps = total_pulses - accel_steps - decel_steps;

    accel_rate  = a;
    pulses_left = total_pulses;
    step_index  = 0;

    uint32_t dt0 = (uint32_t)(sqrtf(2.0f / a) * timerTickHz + 0.5f);
    __HAL_TIM_SET_AUTORELOAD(&htim3, dt0 - 1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dt0 / 2);
    HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_3);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance != TIM3) return;

    if (--pulses_left == 0) {
        HAL_TIM_OC_Stop_IT(&htim3, TIM_CHANNEL_3);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
        return;
    }


    uint32_t dt_ticks;
    if (step_index < accel_steps) {
        float t0 = sqrtf(2.0f * (float)step_index     / accel_rate);
        float t1 = sqrtf(2.0f * (float)(step_index+1) / accel_rate);
        dt_ticks = (uint32_t)((t1 - t0) * timerTickHz + 0.5f);
    }
    else if (step_index < accel_steps + cruise_steps) {
        dt_ticks = (uint32_t)(timerTickHz / cruise_rate + 0.5f);
    }
    else {
        uint32_t i = total_pulses - step_index;
        float t0 = sqrtf(2.0f * (float)i     / accel_rate);
        float t1 = sqrtf(2.0f * (float)(i-1) / accel_rate);
        dt_ticks = (uint32_t)((t0 - t1) * timerTickHz + 0.5f);
    }

    uint32_t hold_steps = (uint32_t)(total_pulses * 0.01f + 0.5f);
    if (pulses_left <= (int32_t)hold_steps) {
        dt_ticks = (uint32_t)(timerTickHz / 10 + 0.5f);
    }

    __HAL_TIM_SET_AUTORELOAD(&htim3, dt_ticks - 1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dt_ticks / 2);

    step_index++;
}

