#include "navigation.h"

systemOfMeasurement currentSysOfMeasurment;
state currentState;
direction currentDirection;

char target[MAX_LENGTH] = {0};
char slowZone[MAX_LENGTH] = {0};
char motor1Range[MAX_LENGTH] = {0};
char motor2Range[MAX_LENGTH] = {0};
char motor1PeakSpeed[MAX_LENGTH] = {0};
char motor1Acceleration[MAX_LENGTH] = {0};
char motor1Pulse[MAX_LENGTH] = {0};
char motor2PeakSpeed[MAX_LENGTH] = {0};
char motor2Acceleration[MAX_LENGTH] = {0};
char motor2Pulse[MAX_LENGTH] = {0};

bool arrowFlag = right;
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
    analog = menu34.child == &menu341 ? disabled : enabled;
    numberLength = units ? 9 : 10;
}

void navigationInit() {
    buildMenuTree();
    
    SSD1309_init();
    SSD1309_drawBitmap(0, 0, 128, 64, current->bitmap);
    if(current == &run) {
        SSD1309_drawText(51, 36, 8, target);
        SSD1309_drawBitmap(53, 51, 13, 7, arrowFlag ? rightArrow : leftArrow);
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
        menu21.child = (current == &menu211 || current == &menu212) ? &menu211 : &menu213;
        menu22.child = (current == &menu221 || current == &menu222) ? &menu221 : &menu223;
        menu24.child = (current == &menu241 || current == &menu242) ? &menu241 : &menu243;
        menu33.child = (current == &menu341 || current == &menu342) ? &menu331 : &menu333;
        menu34.child = (current == &menu51 || current == &menu52) ? &menu341 : &menu343;
        menu4.child = (current == &menu41 || current == &menu42) ? &menu41 : &menu43;
        menu5.child = (current == &menu51 || current == &menu52) ? &menu51 : &menu53;
        current = current->parent;
      }
      else if (c == 'C' && current == &run) {
          uint8_t pos = 0;
          for(uint8_t i = 0; target[i] != '\0'; i++) 
            target[i] = ' ';
          SSD1309_drawText(51, 36, 8, target);
          SSD1309_update();
          while(keypadDecodeKey(raw) != '#' && pos < 10) {
            if(dequeue(&keyQueue, &raw)) {
              if((keypadDecodeKey(raw) >= '0' && keypadDecodeKey(raw) <= '9') || keypadDecodeKey(raw) == '*') {
                char character = keypadDecodeKey(raw);
                if (keypadDecodeKey(raw) == '*')
                  character = '.';
                target[pos++] = character;
              }
              SSD1309_drawText(51, 36, 8, target);
              SSD1309_update();
            }
          }
          if(strtod(target, NULL) > 9000) {
            for(uint8_t i = 0; target[i] != '\0'; i++) 
              target[i] = ' ';
            SSD1309_drawBitmap(48, 36, 78, 7, invalid);
            SSD1309_update();
            HAL_Delay(1000);
          } 
      }
      else if (c == 'D' && current == &run) {
        arrowFlag = !arrowFlag;
  }
  
  SSD1309_drawBitmap(0, 0, 128, 64, current->bitmap);
  if(current == &run) {
    SSD1309_drawText(51, 36, 8, target);
    SSD1309_drawBitmap(53, 51, 13, 7, arrowFlag ? rightArrow : leftArrow);
  }
  SSD1309_update();
  }
}