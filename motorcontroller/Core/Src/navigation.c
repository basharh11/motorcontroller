#include "navigation.h"

menuNode *current = &run;

queue keyQueue;

parameters p;

motor m1;

motor m2;

movementProfile mp1;

char *strings[] = {p.slowZone, p.motor1Range, p.motor2Range, p.motor1PeakSpeed, p.motor1Acceleration, p.motor1Pulse, p.motor2PeakSpeed, p.motor2Acceleration, p.motor2Pulse};

void navigationInit() {
    buildMenuTree();
    parametersInit(&p);
    loadParameters(&p);
    updateAbilityLinkage(&p);
    
    SSD1309_init();
    SSD1309_drawBitmap(0, 0, 128, 64, current->bitmap);

    if(current == &run) {
        SSD1309_drawText(54, 36, 8, p.target);
        SSD1309_drawText(54, 6, 8, p.motor1Position);
        SSD1309_drawBitmap(54, 51, 13, 7, p.arrowDir ? rightArrow : leftArrow);
        SSD1309_drawBitmap(p.units ? 113 : 106, p.units ? 50 : 52, p.units ? 12 : 19, p.units ? 8 : 6, p.units ? in : mm);
    }

    SSD1309_update();  

    queueInit(&keyQueue);     
    keypadInit(&keyQueue); 
    HAL_TIM_Base_Start_IT(&htim2);
    motorInit(&m1, &mp1, &htim3, GPIOC, GPIO_PIN_9, GPIOB, GPIO_PIN_0);
    HAL_TIM_Base_Start_IT(&htim4);
}

void navigationLoop() {
    if(getHomingStatus(&m1) && getHomingReverseStatus(&m1) && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) {
        HAL_TIM_OC_Stop_IT(getHandle(&m1), TIM_CHANNEL_3);
        setHomingStatus(&m1, false);
        setHomingReverseStatus(&m1, false);
        resetStepCount(&m1);
        p.motor1Pos = 0;
    }

    updateParameters(&p);

    SSD1309_drawBitmap(0, 0, 128, 64, current->bitmap);
    if(current == &run) {
        SSD1309_drawText(54, 36, 8, p.target);
        SSD1309_drawBitmap(54, 51, 13, 7, p.arrowDir ? rightArrow : leftArrow);
        SSD1309_drawBitmap(p.units ? 113 : 106, p.units ? 50 : 52, p.units ? 12 : 19, p.units ? 8 : 6, p.units ? in : mm);
        dtoa(p.motor1Position, p.motor1Pos, 4);
        SSD1309_drawText(54, 6, 8, p.motor1Position);
    }

    if(current == &userInput) {
        SSD1309_drawText(6, 6, 8, strings[getInputScreen(current)]);
    }

    SSD1309_update();

    uint8_t raw;

    if(dequeue(&keyQueue, &raw)) {
        char c = keypadDecodeKey(raw);
        if(c == '#' && current->child) {
            updateChildLinkage(current);
            current = current->child;
        } else if (c == '*' && current->parent) {
            updateParentLinkage(current);
            current = current->parent;
        } else if (c == 'A' && current->prev) {
            current = current->prev;
        } else if(c == 'B' && current->next) {
            current = current->next;
        } else if(c == 'A' && current == &run && !getHomingStatus(&m1) && !getMoveStatus(&m1)) {
            moveMotor(&m1);
        } else if(c == 'C' && (current == &userInput || current == &run)) {
            if(current == &userInput)
                strings[getInputScreen(current)][0] = '\0';
            else if(current == &run)
                p.target[0] = '\0';
        } else if(c == 'D' && current == &run && !getHomingStatus(&m1) && !getMoveStatus(&m1)) {
            p.arrowDir = !(p.arrowDir);
        } else if(c == '#' && current == &menu1 && !getHomingStatus(&m1) && !getMoveStatus(&m1)) {
            home(&m1);
        } else if(p.target[0] == '\0' && current == &run && c >= '0' && c <= '9') {
            uint8_t pos = 0;
            bool decimalFlag = false;
            if(c >= '0' && c <= '9') {
                p.target[pos++] = c;
                p.target[pos] = '\0';
                SSD1309_drawText(54, 36, 8, p.target);
                SSD1309_update();
            }
            while(keypadDecodeKey(raw) != '#' && pos < p.numberLength) {   
                if(dequeue(&keyQueue, &raw)) {
                    if(((keypadDecodeKey(raw) >= '0' && keypadDecodeKey(raw) <= '9' && ((pos < (p.units ? 3 : 4)) || p.target[p.numberLength - 5] == '.')) || (keypadDecodeKey(raw) == '*' && !decimalFlag && (pos > 0 && pos < (p.units ? 4 : 5)))) && !(keypadDecodeKey(raw) == '0' && !decimalFlag && pos == 1 && p.target[0] == '0')) {
                        char character = keypadDecodeKey(raw);
                        if(keypadDecodeKey(raw) == '*') {
                            character = '.';
                            decimalFlag = true;
                        }
                        p.target[pos++] = character;
                        if(p.target[pos-1] == '.') 
                            p.numberLength -= ((p.units ? 3 : 4) - (pos - 1));
                        p.target[pos] = '\0';
                    }
                    SSD1309_drawText(54, 36, 8, p.target);
                    SSD1309_update();
                }  
            }
            if(p.target[pos-1] == '.')
                p.target[pos-1] = '\0';
            if((strtof(p.target, NULL) + p.motor1Pos) > strtof(p.motor1Range, NULL)) { 
                p.target[0] = '\0';
                SSD1309_drawBitmap(54, 36, 72, 7, invalid);
                SSD1309_update();
                HAL_Delay(1500);
            } 
        } else if(strings[getInputScreen(current)][0] == '\0' && current == &userInput && c >= '0' && c <= '9')  {
            uint8_t pos = 0;
            bool decimalFlag = false;
            uint8_t idx = getInputScreen(current);
            if(c >= '0' && c <= '9') {
                strings[idx][pos++] = c;
                strings[idx][pos] = '\0';
                SSD1309_drawText(6, 6, 8, strings[idx]);
                SSD1309_update();
            }
            while(keypadDecodeKey(raw) != '#' && pos < p.numberLength) {
                if(dequeue(&keyQueue, &raw)) {
                    if((keypadDecodeKey(raw) >= '0' && keypadDecodeKey(raw) <= '9' && ((pos < (p.units ? 3 : 4)) || strings[idx][p.numberLength - 5] == '.')) || (keypadDecodeKey(raw) == '*' && !decimalFlag && (pos > 0 && pos < (p.units ? 4 : 5)))) {
                        char character = keypadDecodeKey(raw);
                        if(keypadDecodeKey(raw) == '*') {
                            character = '.';
                            decimalFlag = true;
                        }
                        strings[idx][pos++] = character;
                        if(strings[idx][pos-1] == '.') 
                            p.numberLength -= ((p.units ? 3 : 4) - (pos - 1));
                        strings[idx][pos] = '\0';
                    }
                    SSD1309_drawText(6, 6, 8, strings[idx]);
                    SSD1309_update();
                }  
            }
            if(strtof(strings[idx], NULL) > strtof(p.motor1Range, NULL)) {
                strings[idx][0] = '\0';
                SSD1309_drawBitmap(6, 6, 72, 7, invalid);
                SSD1309_update();
                HAL_Delay(1500);
            } 
        }
    }

    saveParameters(&p);
}

void dtoa(char *buf, float val, int precision) {
    if (val < 0.0) {
        *buf++ = '-';
        val = -val;
    }
    int64_t scale = 1;
    for (int i = 0; i < precision; ++i)
        scale *= 10;

    // ε = 1e-(precision+2) is small enough to not affect true truncation,
    // but big enough to push 6666.66659999… up to 6666.66660000
    float eps = 1.0 / pow(10.0, precision + 2);
    float tmp = val * (float)scale + eps;
    int64_t scaled = (int64_t)tmp;

    int64_t ip = scaled / scale;
    int64_t fp = scaled % scale;

    // integer part (reverse into tmpbuf)
    char tmpbuf[32];
    int ti = 0;
    if (ip == 0) tmpbuf[ti++] = '0';
    else {
        while (ip) {
            tmpbuf[ti++] = '0' + (ip % 10);
            ip /= 10;
        }
    }
    while (ti--) *buf++ = tmpbuf[ti];

    // fractional part
    if (precision) {
        *buf++ = '.';
        int64_t div = scale / 10;
        for (int i = 0; i < precision; ++i) {
            int d = fp / div;
            *buf++ = '0' + d;
            fp %= div;
            div /= 10;
        }
    }
    *buf = '\0';
}

