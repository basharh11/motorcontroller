    #include "motor.h"
    #include "main.h"
    #include <math.h>
    
    void setPorts(motor *m, TIM_HandleTypeDef *handle, GPIO_TypeDef *port, uint16_t pin) {
        m->htim = handle;
        m->dirPort = port;
        m->dirPin = pin;
    }

    void setTimerFrequency(motor *m) {
        m->timerFrequency = HAL_RCC_GetPCLK1Freq();
        if((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) 
            m->timerFrequency *= 2;
        m->timerFrequency /= (getHandle(m)->Init.Prescaler + 1);
    }

    void setPulsePerUnit(motor *m) {
        m->pulsePerUnit = strtof(motor1Pulse, NULL);
    }

    void setPeakSpeed(motor *m) {
        m->peakSpeed = strtof(motor1PeakSpeed, NULL);
    }
    
    void setAcceleration(motor *m) {
        m->acceleration = strtof(motor1Acceleration, NULL);
    }

    void setDistance(motor *m) {
        m->distance = strtof(target, NULL);
    }

    TIM_HandleTypeDef* getHandle(const motor *m) {
        return m->htim;
    }

    GPIO_TypeDef* getDirPort(const motor *m) {
        return m->dirPort;
    }

    uint16_t getDirPin(const motor *m) {
        return m->dirPin;
    }

    float getDistance(const motor *m) {
        return m->pulsePerUnit;
    }

    uint32_t getTimerFrequency(const motor *m) {
        return m->timerFrequency;
    }

    float getPulsePerUnit(const motor *m) {
        return m->pulsePerUnit;
    }

    float getPeakSpeed(const motor *m) {
        return m->peakSpeed;
    }
    
    float getAcceleration(const motor *m) {
        return m->pulsePerUnit;
    }

    movementProfile* getMovementProfile(const motor *m) {
        return m->movementProfile;
    }

    bool getHomingStatus(const motor *m) {
        return m->homingActive;
    }

    bool getHomingReverseStatus(const motor *m) {
        return m->homingReverseStarted;
    }

    void moveMotor(motor *m) {
        setPulsePerUnit(m);
        setPeakSpeed(m);
        setAcceleration(m);
        setDistance(m);

        setTotalPulses(getMovementProfile(m), getDistance(m), getPulsePerUnit(m));
        setAccelerationPPSS(getMovementProfile(m), getAcceleration(m), getPulsePerUnit(m));
        setRequestedPeakSpeedPPS(getMovementProfile(m), getPulsePerUnit(m), getDistance(m));
        setMaximumPeakSpeedPPS(getMovementProfile(m), getPulsePerUnit(m));
        setActualPeakSpeedPPS(getMovementProfile(m));
        setRampingDistanceP(getMovementProfile(m));
        setPulseIntervals(getMovementProfile(m));

        HAL_GPIO_WritePin(getDirPort(m), getDirPin(m), arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

        startTimer(m, getMovementProfile(m));
    }

    void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {

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

    void motorOCCallback(motor *m) {
        if(getHandle(m)->Instance != TIM3) 
            return;

        if(getHomingStatus(m)) {
            if(getHomingReverseStatus(m)) {
                setRequestedPeakSpeedPPS(getMovementProfile(m), 2, getPulsePerUnit(m));
                float homing_pps = getRequestedPeakSpeedPPS(m) * getPulsePerUnit(m);
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
        motor1Pos = motor1_step_count / getPulsePerUnit();
    }

    void home(void) {
        float ppu = strtof(motor1Pulse, NULL);
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