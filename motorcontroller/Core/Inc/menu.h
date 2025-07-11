#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include "bitmaps.h"
#include "parameters.h"
#include "enums.h"

typedef struct menuNode {
    const uint8_t *bitmap; // pointer to bitmap to be rendered
    struct menuNode *prev; // previous item on the same level
    struct menuNode *next; // next item on the same level
    struct menuNode *child; // first child (to enter) of this item, or NULL if no child
    struct menuNode *parent; // parent item (to exit), or NULL if top level
} menuNode;

// Run Screen
extern menuNode run;

// Home
extern menuNode menu1;

// Inputs
extern menuNode menu2;

    // Home 1
    extern menuNode menu21;

    extern menuNode menu211;
    extern menuNode menu212;
    extern menuNode menu213;
    extern menuNode menu214;

    // Home 2
    extern menuNode menu22;
    
    extern menuNode menu221;
    extern menuNode menu222;
    extern menuNode menu223;
    extern menuNode menu224;
    
    // Homing Slow Zone (should be removed)
    extern menuNode menu23;

    extern menuNode menu231;

    // Emergency Stop
    extern menuNode menu24;

    extern menuNode menu241;
    extern menuNode menu242;
    extern menuNode menu243;
    extern menuNode menu244;

    // Motor 1 Range
    extern menuNode menu25;

    extern menuNode menu251;

    // Motor 2 Range
    extern menuNode menu26;
    
    extern menuNode menu261;

// Outputs
extern menuNode menu3;

    // Motor 1 Config
    extern menuNode menu31;

        // Motor 1 Peak Speed
        extern menuNode menu311;

        extern menuNode menu3111;

        // Motor 1 Acceleration
        extern menuNode menu312;

        extern menuNode menu3121;

        // Motor 1 Pulse
        extern menuNode menu313;

        extern menuNode menu3131;

    // Motor 2 Config
    extern menuNode menu32;
        
        // Motor 2 Peak Speed
        extern menuNode menu321;

        extern menuNode menu3211;

        // Motor 2 Acceleration
        extern menuNode menu322;

        extern menuNode menu3221;

        // Motor 2 Pulse
        extern menuNode menu323;

        extern menuNode menu3231;

    // Relay 1
    extern menuNode menu33;

    extern menuNode menu331;
    extern menuNode menu332;
    extern menuNode menu333;
    extern menuNode menu334;

    // Relay 2
    extern menuNode menu34;

    extern menuNode menu341;
    extern menuNode menu342;
    extern menuNode menu343;
    extern menuNode menu344;

// 0-10V Analog
extern menuNode menu4;
    
extern menuNode menu41;
extern menuNode menu42;
extern menuNode menu43;
extern menuNode menu44;

// Units
extern menuNode menu5;

extern menuNode menu51;
extern menuNode menu52;
extern menuNode menu53;
extern menuNode menu54;

extern menuNode userInput;

extern menuNode disabledCheck;
extern menuNode enabled;
extern menuNode enabledCheck;
extern menuNode disabled;

const uint8_t* getBitmap(const menuNode *name);
menuNode* getPrev(const menuNode *name);
menuNode* getNext(const menuNode *name);
menuNode* getChild(const menuNode *name);
menuNode* getParent(const menuNode *name);

void setPrev(menuNode name);
void setNext(menuNode name);
void setChild(menuNode name);
void setParent(menuNode name);

void updateInputLinkage(menuNode *name);
void updateAbilityLinkage(menuNode *name, state s);

void buildMenuTree();

#endif