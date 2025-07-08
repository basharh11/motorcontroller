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

uint32_t getFirstTick(const movementProfile *mp) {
    return mp->firstTick;
}

uint32_t getNextTick(const movementProfile *mp) {
    return mp->nextTick;
}

void setTotalPulses(movementProfile *mp, uint32_t distance, uint32_t pulsePerUnit) {
    mp->totalPulses = (uint32_t)roundf(distance * pulsePerUnit);
}

void setAccelerationPPSS(movementProfile *mp, uint32_t acceleration, uint32_t pulsePerUnit) {
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

void setPulseIntervals(motor *m, movementProfile *mp) {
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

    mp->firstTick = (uint32_t)(sqrtf(2.0f / getAccelerationPPSS(mp)) * getTimerFrequency(m) + 0.5f);
}

void setHomingPulseIntervals(motor *m, movementProfile *mp) {
    setRequestedPeakSpeedPPS(getMovementProfile(m), 10, getPulsePerUnit(m));
    mp->firstTick = (uint32_t)(getTimerFrequency(m) / getActualPeakSpeedPPS(mp) * getPulsePerUnit(m) + 0.5f);
}

void incrementStepIndex(movementProfile *mp) {
    mp->stepIndex++;
}

void setNextTick(motor *m, movementProfile *mp) {
    if(getStepIndex(mp) < getAccelSteps(mp)) {
        float t0 = sqrtf(2.0f * getStepIndex(mp) / getAccelerationPPSS(mp));
        float t1 = sqrtf(2.0f * (getStepIndex(mp) + 1) / getAccelerationPPSS(mp));
        mp->nextTick = (uint32_t)((t1 - t0) * getTimerFrequency(m) + 0.5f);
    } else if (getStepIndex(mp) < getAccelSteps(mp) + getPeakSteps(mp)) {
        mp->nextTick = (uint32_t)(getTimerFrequency(m) / getActualPeakSpeedPPS(mp) + 0.5f);
    } else {
        uint32_t i = getTotalPulses(mp) - getStepIndex(mp);
        float t0 = sqrtf(2.0f * i / getAccelerationPPSS(mp));
        float t1 = sqrtf(2.0f * (i - 1) / getAccelerationPPSS(mp));
        mp->nextTick = (uint32_t)((t0 - t1) * getTimerFrequency(m) + 0.5f);
    }
}


    