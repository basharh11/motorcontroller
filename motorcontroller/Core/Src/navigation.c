#include "navigation.h"

systemOfMeasurement currentSysOfMeasurment;
state currentState;
direction currentDirection;

char target[MAX_LENGTH] = {0};
char *parameters[] = {slowZone, motor1Range, motor2Range, motor1PeakSpeed, motor1Acceleration, motor1Pulse, motor2PeakSpeed, motor2Acceleration, motor2Range};
char slowZone[MAX_LENGTH] = {0};
char motor1Range[MAX_LENGTH] = {0};
char motor2Range[MAX_LENGTH] = {0};
char motor1PeakSpeed[MAX_LENGTH] = {0};
char motor1Acceleration[MAX_LENGTH] = {0};
char motor1Pulse[MAX_LENGTH] = {0};
char motor2PeakSpeed[MAX_LENGTH] = {0};
char motor2Acceleration[MAX_LENGTH] = {0};
char motor2Pulse[MAX_LENGTH] = {0};

bool arrowDir = right;
bool home1 = disabled;
bool home2 = disabled;
bool emergencyStop = disabled;
bool relay1 = disabled;
bool relay2 = disabled;
bool analog = disabled;
bool units = metric;

uint8_t numberLength = 9;

MenuNode *current = &run;

queue keyQueue;

void updateParameters() {
    home1 = menu21.child == &menu211 ? disabled : enabled;
    home2 = menu22.child == &menu221 ? disabled : enabled;
    emergencyStop = menu24.child == &menu241 ? disabled : enabled;
    relay1 = menu33.child == &menu331 ? disabled : enabled;
    relay2 = menu34.child == &menu341 ? disabled : enabled;
    analog = menu4.child == &menu41 ? disabled : enabled;
    units = menu5.child == &menu51 ? metric : imperial;
    numberLength = units ? 9 : 10;
}

void navigationInit() {
    buildMenuTree();
    
    SSD1309_init();
    SSD1309_drawBitmap(0, 0, 128, 64, current->bitmap);
    if(current == &run) {
        SSD1309_drawText(51, 36, 8, target);
        SSD1309_drawBitmap(53, 51, 13, 7, arrowDir ? rightArrow : leftArrow);
    }
    SSD1309_update();  

    queueInit(&keyQueue);     
    keypadInit(&keyQueue); 
}

void navigationLoop() {
    uint8_t raw;

    if(dequeue(&keyQueue, &raw)) {
        char c = keypadDecodeKey(raw);

        if (c == 'B' && current->next) {
            current = current->next;
        }
        else if (c == 'A' && current->prev) {
            current = current->prev;
        }
        else if (c == '#' && current->child) {
            current = current->child;
        }
        else if (c == '*' && current->parent) {
            if(current == &menu211 || current == &menu212)
                current->parent->child = &menu211 ;
            else if(current == &menu213 || current == &menu214)
                current->parent->child = &menu213; 
            else if(current == &menu221 || current == &menu222)
                current->parent->child = &menu221; 
            else if(current == &menu223 || current == &menu224)
                current->parent->child = &menu223; 
            else if(current == &menu241 || current == &menu242)
                current->parent->child = &menu241; 
            else if(current == &menu243 || current == &menu244)
                current->parent->child = &menu243; 
            else if(current == &menu331 || current == &menu332)
                current->parent->child = &menu331; 
            else if(current == &menu333 || current == &menu334)
                current->parent->child = &menu333; 
            else if(current == &menu41 || current == &menu42)
                current->parent->child = &menu41; 
            else if(current == &menu43 || current == &menu44)
                current->parent->child = &menu43; 
            else if(current == &menu51 || current == &menu52)
                current->parent->child = &menu51; 
            else if(current == &menu53 || current == &menu54)
                current->parent->child = &menu53;
            current = current->parent;
        }
        else if (c == 'C' && current == &run) {
            uint8_t pos = 0;
            target[0] = '\0';
            SSD1309_drawText(51, 36, 8, target);
            SSD1309_update();
            while(keypadDecodeKey(raw) != '#' && pos < 10) {
                if(dequeue(&keyQueue, &raw)) {
                    if((keypadDecodeKey(raw) >= '0' && keypadDecodeKey(raw) <= '9') || keypadDecodeKey(raw) == '*') {
                        char character = keypadDecodeKey(raw);
                        if (keypadDecodeKey(raw) == '*')
                            character = '.';
                        target[pos++] = character;
                        target[pos] = '\0';
                    }
                    SSD1309_drawText(51, 36, 8, target);
                    SSD1309_update();
                }
            }
            pos = 0;
            if(strtod(target, NULL) > 9000) {
                target[0] = '\0';
                SSD1309_drawBitmap(48, 36, 78, 7, invalid);
                SSD1309_update();
                HAL_Delay(1000);
            } 
        }
        else if(c == 'D' && current == &run) {
            arrowDir = !arrowDir;
        }
        else if(c == 'C' && isInputScreen()) {
            parameters[selectInputScreen()][0] = '\0';
        }
        else if(parameters[selectInputScreen()][0] == '\0' && isInputScreen()) {
            uint8_t pos = 0;
            uint8_t idx = selectInputScreen();
            parameters[idx][0] = '\0';
            while(keypadDecodeKey(raw) != '#' && pos < numberLength) {
                if(dequeue(&keyQueue, &raw)) {
                    if((keypadDecodeKey(raw) >= '0' && keypadDecodeKey(raw) <= '9') || keypadDecodeKey(raw) == '*') {
                        char character = keypadDecodeKey(raw);
                        if (keypadDecodeKey(raw) == '*')
                            character = '.';
                        parameters[idx][pos++] = character;
                        parameters[idx][pos] = '\0';
                    }
                    SSD1309_drawText(6, 6, 8, parameters[idx]);
                    SSD1309_update();
                }  
            }
            if(strtod(parameters[idx], NULL) > 9000) {
                parameters[idx][0] = '\0';
                SSD1309_drawBitmap(4, 6, 78, 7, invalid);
                SSD1309_update();
                HAL_Delay(1000);
            } 
        }
    }

    SSD1309_drawBitmap(0, 0, 128, 64, current->bitmap);
    if(current == &run) {
        SSD1309_drawText(51, 36, 8, target);
        SSD1309_drawBitmap(53, 51, 13, 7, arrowDir ? rightArrow : leftArrow);
    }
    if(isInputScreen()) {
        SSD1309_drawText(6, 6, 8, parameters[selectInputScreen()]);
    }
    SSD1309_update();
}

uint8_t selectInputScreen() {
    uint8_t select;
    if(current == &menu231)
        select = 0;
    else if(current == &menu251)
        select = 1;
    else if(current == &menu261)
        select = 2;
    else if(current == &menu321)
        select = 3;
    else if(current == &menu322)
        select = 4;
    else if(current == &menu323)
        select = 5;
    else if(current == &menu321)
        select = 6;
    else if(current == &menu322)
        select = 7;
    else if(current == &menu322)
        select = 8;
    return select;
}

bool isInputScreen() {
    bool inputScreen = false;
    if(current == &menu231)
        inputScreen = true;
    else if(current == &menu251)
        inputScreen = true;
    else if(current == &menu261)
        inputScreen = true;
    else if(current == &menu321)
        inputScreen = true;
    else if(current == &menu322)
        inputScreen = true;
    else if(current == &menu323)
        inputScreen = true;
    else if(current == &menu321)
        inputScreen = true;
    else if(current == &menu322)
        inputScreen = true;
    else if(current == &menu322)
        inputScreen = true;
    return inputScreen;
}