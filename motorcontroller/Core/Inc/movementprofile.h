#ifndef MOVEMENTPROFILE_H
#define MOVEMENTPROFILE_H

#include "motor.h"

typedef struct motor motor;

typedef struct movementProfile {
    int32_t remainingPulses;
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

    uint32_t firstTick;
    uint32_t nextTick;

    uint32_t homingTicks;
} movementProfile;

int32_t getRemainingPulses(const movementProfile *mp);
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
uint32_t getFirstTick(const movementProfile *mp);
uint32_t getNextTick(const movementProfile *mp);
uint32_t getHomingTicks(const movementProfile *mp);

void setTotalPulses(movementProfile *mp, float distance, float pulsePerUnit);
void setAccelerationPPSS(movementProfile *mp, float acceleration, float pulsePerUnit);
void setRequestedPeakSpeedPPS(movementProfile *mp, float peakSpeed, float pulsePerUnit);
void setMaximumPeakSpeedPPS(movementProfile *mp, uint32_t totalPulses);
void setActualPeakSpeedPPS(movementProfile *mp);
void setRampingDistanceP(movementProfile *mp);
void setPulseIntervals(motor *m, movementProfile *mp);
void setHomingPulseIntervals(motor *m, movementProfile *mp);
void incrementStepIndex(movementProfile *mp);
void setNextTick(motor *m, movementProfile *mp);
void decrementRemainingPulses(movementProfile *mp);

#endif