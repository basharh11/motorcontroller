#ifndef MOTOR_H
#define MOTOR_H

#include "navigation.h"
#include "movementprofile.h"

typedef struct movementProfile movementProfile;

typedef struct motor {
    TIM_HandleTypeDef *htim;
    GPIO_TypeDef *dirPort;
    uint16_t dirPin;

    uint32_t timerFrequency;
    float pulsePerUnit;
    float peakSpeed;
    float acceleration;
    float distance;

    movementProfile *movementProfile;

    bool homingActive;
    bool homingReverseStarted;
    uint32_t homing_dt_ticks;
    int32_t stepCount;
    float motor1_steps_per_unit;
} motor;

extern bool arrowDir;
extern bool home1;
extern bool home2;
extern bool emergencyStop;
extern bool relay1;
extern bool relay2;
extern bool analog;
extern bool units;
extern bool lastUnits;

extern char motor1Pulse[MAX_LENGTH];
extern volatile int32_t motor1_step_count;
extern float motor1_steps_per_unit;
extern float motor1Pos;

TIM_HandleTypeDef *getHandle(const motor *m);
GPIO_TypeDef *getDirPort(const motor *m);
uint16_t getDirPin(const motor *m);
uint32_t getTimerFrequency(const motor *m);
float getPulsePerUnit(const motor *m);
float getPeakSpeed(const motor *m);
float getAcceleration(const motor *m);
float getDistance(const motor *m);
movementProfile* getMovementProfile(const motor *m);
bool getHomingStatus(const motor *m);
bool getHomingReverseStatus(const motor *m);

void setPorts(motor *m, TIM_HandleTypeDef *htim, GPIO_TypeDef *dirPort, uint16_t dirPin);
void setTimerFrequency(motor *m);
void setPulsePerUnit(motor *m);
void setPeakSpeed(motor *m);
void setAcceleration(motor *m);
void setDistance(motor *m);

void moveMotor(motor *m);

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *motor);

void home(void);

#endif