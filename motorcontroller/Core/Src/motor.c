#include "motor.h"

void motorInit(motor *m, movementProfile *mp, TIM_HandleTypeDef *handle, GPIO_TypeDef *dport, uint16_t dpin, GPIO_TypeDef *hport, uint16_t hpin) {
    m->htim = handle;
    m->dirPort = dport;
    m->dirPin = dpin;
    m->homePort = hport;
    m->homePin = hpin;
    m->movementProfile = mp;
    m->moveActive = false;
    m->homingActive = false;
    m->homingReverseStarted = false;
    setTimerFrequency(m);
    HAL_TIM_Base_Start(handle);
    resetStepCount(m);
}

void setTimerFrequency(motor *m) {
    m->timerFrequency = HAL_RCC_GetPCLK1Freq();
    if((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) 
        m->timerFrequency *= 2;
    m->timerFrequency /= (getHandle(m)->Init.Prescaler + 1);
}

void setPulsePerUnit(motor *m) {
    m->pulsePerUnit = strtof(p.motor1Pulse, NULL);
}

void setPeakSpeed(motor *m) {
    m->peakSpeed = p.units ? (strtof(p.motor1PeakSpeed, NULL) / 2.54) : strtof(p.motor1PeakSpeed, NULL);
}

void setAcceleration(motor *m) {
    m->acceleration = p.units ? (strtof(p.motor1Acceleration, NULL) / 2.54) : strtof(p.motor1Acceleration, NULL);
}

void setDistance(motor *m) {
    m->distance = p.units ? (strtof(p.target, NULL) / 2.54) : strtof(p.target, NULL);
}

void setMoveStatus(motor *m, bool status) {
    m->moveActive = status;
}

void setHomingStatus(motor *m, bool status) {
    m->homingActive = status;
}

void setHomingReverseStatus(motor *m, bool status) {
    m->homingReverseStarted = status;
}

void resetStepCount(motor *m) {
    m->stepCount = 0;
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

uint32_t getTimerFrequency(const motor *m) {
    return m->timerFrequency;
}

float getDistance(const motor *m) {
    return m->distance;
}

float getPulsePerUnit(const motor *m) {
    return m->pulsePerUnit;
}

float getPeakSpeed(const motor *m) {
    return m->peakSpeed;
}

float getAcceleration(const motor *m) {
    return m->acceleration;
}

movementProfile* getMovementProfile(const motor *m) {
    return m->movementProfile;
}

bool getMoveStatus(const motor *m) {
    return m->moveActive;
}

bool getHomingStatus(const motor *m) {
    return m->homingActive;
}

bool getHomingReverseStatus(const motor *m) {
    return m->homingReverseStarted;
}

int32_t getStepCount(const motor *m) {
    return m->stepCount;
}

void moveMotor(motor *m) {
    movementProfile *mp = getMovementProfile(m);
    
    setPulsePerUnit(m);
    setPeakSpeed(m);
    setAcceleration(m);
    setDistance(m);

    setTotalPulses(mp, getDistance(m), getPulsePerUnit(m));
    setAccelerationPPSS(mp, getAcceleration(m), getPulsePerUnit(m));
    setRequestedPeakSpeedPPS(mp, getPeakSpeed(m), getPulsePerUnit(m));
    setMaximumPeakSpeedPPS(mp, getTotalPulses(mp));
    setActualPeakSpeedPPS(mp);
    setRampingDistanceP(mp);
    setPulseIntervals(m, mp);

    HAL_GPIO_WritePin(getDirPort(m), getDirPin(m), p.arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

    __HAL_TIM_SET_AUTORELOAD(getHandle(m), getFirstTick(mp) - 1);
    __HAL_TIM_SET_COMPARE(getHandle(m), TIM_CHANNEL_3, getFirstTick(mp) / 2);

    setMoveStatus(m, true);

    HAL_TIM_OC_Start_IT(getHandle(m), TIM_CHANNEL_3);
}

void home(motor *m) {
    movementProfile *mp = getMovementProfile(m);
    TIM_HandleTypeDef *handle = getHandle(m);

    setPulsePerUnit(m); 

    setHomingPulseIntervals(m, mp);

    setHomingStatus(m, true);
    setHomingReverseStatus(m, false);

    HAL_GPIO_WritePin(getDirPort(m), getDirPin(m), p.arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);

    __HAL_TIM_SET_AUTORELOAD(handle, getHomingTicks(mp) - 1);
    __HAL_TIM_SET_COMPARE(handle, TIM_CHANNEL_3, getHomingTicks(mp) / 2);

    HAL_TIM_OC_Start_IT(handle, TIM_CHANNEL_3);
}

void motorOCCallback(motor *m) {
    movementProfile *mp = getMovementProfile(m);
    TIM_HandleTypeDef *handle = getHandle(m);

    if(handle->Instance != TIM3) 
        return;

    if(m->homingActive) {
        if(m->homingReverseStarted) {
            setHomingPulseIntervals(m, mp);
        }
        __HAL_TIM_SET_AUTORELOAD(handle, getHomingTicks(mp) - 1);
        __HAL_TIM_SET_COMPARE(handle, TIM_CHANNEL_3, getHomingTicks(mp) / 2);
        return;
    }
    
    if(getRemainingPulses(mp) == 0) {
        setMoveStatus(m, false);
        HAL_TIM_OC_Stop_IT(getHandle(m), TIM_CHANNEL_3);
        return;
    }

    decrementRemainingPulses(mp);

    setNextTick(m, mp);

    __HAL_TIM_SET_AUTORELOAD(handle, getNextTick(mp) - 1);
    __HAL_TIM_SET_COMPARE(handle, TIM_CHANNEL_3, getNextTick(mp) / 2);

    incrementStepIndex(mp);

    m->stepCount += (p.arrowDir ? -1 : +1);
    p.motor1Pos = m->stepCount / m->pulsePerUnit;
}

void homeSensor(motor *m) {
    if(m->homePin != GPIO_PIN_0)    
        return;  

    if(!m->homingActive)            
        return;  

    if(HAL_GPIO_ReadPin(m->homePort, m->homePin) == GPIO_PIN_SET && !m->homingReverseStarted) {
        p.arrowDir = !p.arrowDir;
        HAL_GPIO_WritePin(m->dirPort, m->dirPin, p.arrowDir ? GPIO_PIN_RESET : GPIO_PIN_SET);
        m->homingReverseStarted = true;
    }             
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM3)
        motorOCCallback(&m1);
}

void switchEXTIHandler(uint16_t GPIO_Pin) {
    if(GPIO_Pin == getHomePin(&m1))
        homeSensor(&m1);
}

