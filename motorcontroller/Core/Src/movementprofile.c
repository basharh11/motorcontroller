#include "movementprofile.h"

uint32_t getTotalPulses(const movementProfile *mp) {
    return mp->totalPulses;
}

int32_t getRemainingPulses(const movementProfile *mp) {
    return mp->remainingPulses;
}

float getAccelerationPPSS(const movementProfile *mp) {
    return mp->accelerationPPSS;
}

float getRequestedPeakSpeedPPS(const movementProfile *mp) {
    return mp->requestedPeakSpeedPPS;
}

float getMaximumPeakSpeedPPS(const movementProfile *mp) {
    return mp->maximumPeakSpeedPPS;
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

uint32_t getHomingTicks(const movementProfile *mp) {
    return mp->homingTicks;
}

void setTotalPulses(movementProfile *mp, float distance, float pulsePerUnit) {
    mp->totalPulses = (uint32_t)roundf(distance * pulsePerUnit);
}

void setAccelerationPPSS(movementProfile *mp, float acceleration, float pulsePerUnit) {
    mp->accelerationPPSS = acceleration * pulsePerUnit;
}

void setRequestedPeakSpeedPPS(movementProfile *mp, float peakSpeed, float pulsePerUnit) {
    mp->requestedPeakSpeedPPS = peakSpeed * pulsePerUnit;
}

void setMaximumPeakSpeedPPS(movementProfile *mp, uint32_t totalPulses) {
    mp->maximumPeakSpeedPPS = sqrtf(2.0f * getAccelerationPPSS(mp) * totalPulses);
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
    mp->homingTicks = (uint32_t)(m->timerFrequency / (((m->homingReverseStarted) ? 2 : 10) * m->pulsePerUnit) + 0.5f);
}

void incrementStepIndex(movementProfile *mp) {
    mp->stepIndex++;
}

void decrementRemainingPulses(movementProfile *mp) {
    mp->remainingPulses--;
}

void setNextTick(motor *m, movementProfile *mp) {
    uint32_t stepIndex = m->movementProfile->stepIndex;
    uint32_t accelSteps = m->movementProfile->accelSteps;
    float accelerationPPPS = m->movementProfile->accelerationPPSS;
    uint32_t timFreq = m->timerFrequency;
    uint32_t peakSteps = m->movementProfile->peakSteps;
    float actualPeakSpeed = m->movementProfile->actualPeakSpeedPPS;
    uint32_t totalPulses = m->movementProfile->totalPulses;

    if(stepIndex < accelSteps) {
        float t0 = sqrtf(2.0f * stepIndex / accelerationPPPS);
        float t1 = sqrtf(2.0f * (stepIndex + 1) / accelerationPPPS);
        mp->nextTick = (uint32_t)((t1 - t0) * timFreq + 0.5f);
    } else if (stepIndex < accelSteps + peakSteps) {
        mp->nextTick = (uint32_t)(timFreq / actualPeakSpeed + 0.5f);
    } else {    
        uint32_t i = totalPulses - stepIndex;
        float t0 = sqrtf(2.0f * i / accelerationPPPS);
        float t1 = sqrtf(2.0f * (i - 1) / accelerationPPPS);
        mp->nextTick = (uint32_t)((t0 - t1) * timFreq + 0.5f);
    }
}



    