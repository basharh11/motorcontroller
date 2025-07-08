    #include "motor.h"
    #include "main.h"
    #include <math.h>
    
    void setPorts(motor *m, TIM_HandleTypeDef *handle, GPIO_TypeDef *dport, uint16_t dpin, GPIO_TypeDef *hport, uint16_t hpin) {
        m->htim = handle;
        m->dirPort = dport;
        m->dirPin = dpin;
        m->homePort = hport;
        m->homePin = hpin;
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

    void startTimer(motor *m) {
        __HAL_TIM_SET_AUTORELOAD(getHandle(m), getFirstTick(getMovementProfile(m)) - 1);
        __HAL_TIM_SET_COMPARE(getHandle(m), TIM_CHANNEL_3, getFirstTick(getMovementProfile(m)) / 2);

        HAL_TIM_OC_Start_IT(getHandle(m), TIM_CHANNEL_3);
    }

    void setHomingStatus(motor *m, bool status) {
        m->homingActive = status;

    }

    void setHomingReverseStatus(motor *m, bool status) {
        m->homingReverseStarted = status;
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

    GPIO_TypeDef* getHomePort(const motor *m) {
        return m->homePort;
    }

    uint16_t getHomePin(const motor *m) {
        return m->homePin;
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

    uint32_t getStepCount(const motor *m) {
        return m->stepCount;
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
        setPulseIntervals(m, getMovementProfile(m));

        HAL_GPIO_WritePin(getDirPort(m), getDirPin(m), arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

        startTimer(m);
    }

    void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
        if(getHandle(&motor1) == htim)
            motorOCCallback(&motor1);
    }

    void switchEXTIHandler(motor *m) {
        if(getHomePin(m) != GPIO_PIN_0)    
            return;  

        if(!getHomingStatus(m))            
            return;  

        if(HAL_GPIO_ReadPin(getHomePort(m), getHomePin(m)) == GPIO_PIN_SET && !getHomingReverseStatus(m)) {
            arrowDir = !arrowDir;
            HAL_GPIO_WritePin(getDirPort(m), getDirPin(m), arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);
            setHomingReverseStatus(m, true);
        }             
    }

    void motorOCCallback(motor *m) {
        if(getHandle(m)->Instance != TIM3) 
            return;

        if(getHomingStatus(m)) {
            if(getHomingReverseStatus(m)) {
                setHomingPulseIntervals(m, getMovementProfile(m));
            }
            __HAL_TIM_SET_AUTORELOAD(getHandle(m), getFirstTick(getMovementProfile(m)) - 1);
            __HAL_TIM_SET_COMPARE(getHandle(m), TIM_CHANNEL_3, getFirstTick(getMovementProfile(m)) / 2);
            return;
        }
        
        uint32_t remainingPulses = getRemainingPulses(getMovementProfile(m));
        if (--remainingPulses <= 0) {
            HAL_TIM_OC_Stop_IT(getHandle(m), TIM_CHANNEL_3);
            return;
        }

        setNextTick(m, getMovementProfile(m));
    
        __HAL_TIM_SET_AUTORELOAD(getHandle(m), getNextTick(getMovementProfile(m)) - 1);
        __HAL_TIM_SET_COMPARE(getHandle(m), TIM_CHANNEL_3, getNextTick(getMovementProfile(m)) / 2);

        incrementStepIndex(getMovementProfile(m));

        m->stepCount += (arrowDir ? -1 : +1);
        motor1Pos = getStepCount(m) / getPulsePerUnit(m);
    }

    void home(motor *m) {
        setHomingPulseIntervals(m, getMovementProfile(m));

        setHomingStatus(m, true);
        setHomingReverseStatus(m, false);

        HAL_GPIO_WritePin(getDirPort(m), getDirPin(m), arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

        __HAL_TIM_SET_AUTORELOAD(getHandle(m), getFirstTick(getMovementProfile(m)) - 1);
        __HAL_TIM_SET_COMPARE(getHandle(m), TIM_CHANNEL_3, getFirstTick(getMovementProfile(m)) / 2);

        HAL_TIM_OC_Start_IT(getHandle(m), TIM_CHANNEL_3);
    }