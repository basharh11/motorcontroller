#ifndef MOVEMENTPROFILE_H
#define MOVEMENTPROFILE_H

#include "motor.h"

typedef struct motor motor;

typedef struct movementProfile {
    uint32_t remainingPulses;
    uint32_t totalPulses;
    float accelerationPPSS;
    float requestedPeakSpeedPPS;
    float maximumPeakSpeedPPS;
    float actualPeakSpeedPPS;
    float rampingDistanceP;
    uint32_t stepIndex;
    uint32_t accelSteps;
    uint32_t peakSteps;
    uint32_t decelSteps;
} movementProfile;

uint32_t getRemainingPulses(const movementProfile *mp);
uint32_t getTotalPulses(const movementProfile *mp);
float getAccelerationPPSS(const movementProfile *mp);
float getRequestedPeakSpeedPPS(const movementProfile *mp);
float getMaximumPeakSpeedPPS(const movementProfile *mp);
float getActualPeakSpeedPPS(const movementProfile *mp);
float getRampingDistanceP(const movementProfile *mp);
uint32_t getStepIndex(const movementProfile *mp);
uint32_t getAccelSteps(const movementProfile *mp);
uint32_t getPeakSteps(const movementProfile *mp);
uint32_t getDecelSteps(const movementProfile *mp);

void setTotalPulses(movementProfile *mp, uint32_t distance, uint32_t pulsePerUnit);
void setAccelerationPPSS(movementProfile *mp, uint32_t acceleration, uint32_t pulsePerUnit);
void setRequestedPeakSpeedPPS(movementProfile *mp, uint32_t peakSpeed, uint32_t pulsePerUnit);
void setMaximumPeakSpeedPPS(movementProfile *mp, uint32_t pulsePerUnit);
void setActualPeakSpeedPPS(movementProfile *mp);
void setRampingDistanceP(movementProfile *mp);
void setPulseIntervals(movementProfile *mp);

void startTimer(motor *motor, movementProfile *mp);

#endif