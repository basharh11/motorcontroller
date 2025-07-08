#include "movementprofile.h"
#include "motor.h"

uint32_t getTotalPulses(const movementProfile *mp) {
    return mp->accelerationPPSS;
}

uint32_t getRemainingPulses(const movementProfile *mp) {
    return mp->remainingPulses;
}

float getAccelerationPPSS(const movementProfile *mp) {
    return mp->accelerationPPSS;
}

float getRequestedPeakSpeedPPS(const movementProfile *mp) {
    return mp->requestedPeakSpeedPPS;
}

float getMaximumPeakSpeedPPS(const movementProfile *mp) {
    return mp->requestedPeakSpeedPPS;
}

float getActualPeakSpeedPPS(const movementProfile *mp) {
    return mp->actualPeakSpeedPPS;
}

float getRampingDistanceP(const movementProfile *mp) {
    return mp->rampingDistanceP;
}

uint32_t getStepIndex(const movementProfile *mp) {
    return mp->stepIndex;
}

uint32_t getAccelSteps(const movementProfile *mp) {
    return mp->accelSteps;
}

uint32_t getPeakSteps(const movementProfile *mp) {
    return mp->peakSteps;
}

uint32_t getDecelSteps(const movementProfile *mp) {
    return mp->decelSteps;
}

void setTotalPulses(movementProfile *mp, uint32_t distance, uint32_t pulsePerUnit) {
    mp->totalPulses = (uint32_t)roundf(distance * pulsePerUnit);
}

void setAccelerationPPPS(movementProfile *mp, uint32_t acceleration, uint32_t pulsePerUnit) {
    mp->accelerationPPSS = acceleration * pulsePerUnit;
}

void setRequestedPeakSpeedPPS(movementProfile *mp, uint32_t peakSpeed, uint32_t pulsePerUnit) {
    mp->requestedPeakSpeedPPS = peakSpeed * pulsePerUnit;
}

void setMaximumPeakSpeedPPS(movementProfile *mp, uint32_t pulsePerUnit) {
    mp->maximumPeakSpeedPPS = sqrtf(2.0f * getAccelerationPPSS(mp) * pulsePerUnit);
}

void setActualPeakSpeedPPS(movementProfile *mp) {
    mp->actualPeakSpeedPPS = getRequestedPeakSpeedPPS(mp) < getMaximumPeakSpeedPPS(mp) ? getRequestedPeakSpeedPPS(mp) : getMaximumPeakSpeedPPS(mp);
}

void setRampingDistanceP(movementProfile *mp) {
    mp->rampingDistanceP = (getActualPeakSpeedPPS(mp) * getActualPeakSpeedPPS(mp)) / (2.0f * getAccelerationPPSS(mp));
}

void setPulseIntervals(movementProfile *mp) {
    if(getRampingDistanceP(mp) >= getTotalPulses(mp)) {
        mp->accelSteps = getTotalPulses(mp) / 2;
        mp->peakSteps = 0;
    } else {
        mp->accelSteps = (uint32_t)ceilf(getRampingDistanceP(mp));
        mp->peakSteps = getTotalPulses(mp) - 2 * getAccelSteps(mp);
    }
    mp->decelSteps = getAccelSteps(mp);

    mp->remainingPulses = getTotalPulses(mp);
    mp->stepIndex = 0;
}

void startTimer(motor* motor, movementProfile *mp) {
    uint32_t dt0 = (uint32_t)(sqrtf(2.0f / getAccelerationPPSS(mp)) * getTimerFrequency(motor) + 0.5f);

    __HAL_TIM_SET_AUTORELOAD(getHandle(motor), dt0 - 1);
    __HAL_TIM_SET_COMPARE(getHandle(motor), TIM_CHANNEL_3, dt0 / 2);

    HAL_TIM_OC_Start_IT(getHandle(motor), TIM_CHANNEL_3);
}

    