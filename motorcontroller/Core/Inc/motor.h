#ifndef MOTOR_H
#define MOTOR_H

#include "navigation.h"
#include "movementprofile.h"

typedef struct movementProfile movementProfile;

typedef struct motor {
    TIM_HandleTypeDef *htim;
    GPIO_TypeDef *dirPort;
    uint16_t dirPin;
    GPIO_TypeDef *homePort;
    uint16_t homePin;

    uint32_t timerFrequency;
    float pulsePerUnit;
    float peakSpeed;
    float acceleration;
    float distance;

    movementProfile *movementProfile;

    bool moveActive;
    bool homingActive;
    bool homingReverseStarted;
    
    int32_t stepCount;
} motor;

extern motor m1;
extern motor m2;

extern parameters p;

TIM_HandleTypeDef *getHandle(const motor *m);
GPIO_TypeDef *getDirPort(const motor *m);
uint16_t getDirPin(const motor *m);
uint32_t getTimerFrequency(const motor *m);
float getPulsePerUnit(const motor *m);
float getPeakSpeed(const motor *m);
float getAcceleration(const motor *m);
float getDistance(const motor *m);
movementProfile* getMovementProfile(const motor *m);
bool getMoveStatus(const motor *m);
bool getHomingStatus(const motor *m);
bool getHomingReverseStatus(const motor *m);
int32_t getStepCount(const motor *m);

void motorInit(motor *m, movementProfile *mp, TIM_HandleTypeDef *htim, GPIO_TypeDef *dirPort, uint16_t dirPin, GPIO_TypeDef *homePort, uint16_t homePin);
void setTimerFrequency(motor *m);
void setPulsePerUnit(motor *m);
void setPeakSpeed(motor *m);
void setAcceleration(motor *m);
void setDistance(motor *m);
void setMoveStatus(motor *m, bool status);
void setHomingStatus(motor *m, bool status);
void setHomingReverseStatus(motor *m, bool status);
void resetStepCount(motor *m);

void moveMotor(motor *m);
void home(motor *m);

void motorOCCallback(motor *m);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *motor);

#endif