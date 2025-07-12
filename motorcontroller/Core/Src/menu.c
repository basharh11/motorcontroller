#include "menu.h"

// Run Screen
menuNode run;

// Home
menuNode menu1;

// Inputs
menuNode menu2;

    // Home 1
    menuNode menu21;

    // Home 2
    menuNode menu22;
    
    // Homing Slow Zone (should be removed)
    menuNode menu23;

    // Emergency Stop
    menuNode menu24;

    // Motor 1 Range
    menuNode menu25;

    // Motor 2 Range
    menuNode menu26;

// Outputs
menuNode menu3;

    // Motor 1 Config
    menuNode menu31;

        // Motor 1 Peak Speed
        menuNode menu311;

        // Motor 1 Acceleration
        menuNode menu312;

        // Motor 1 Pulse
        menuNode menu313;

    // Motor 2 Config
    menuNode menu32;
        
        // Motor 2 Peak Speed
        menuNode menu321;

        // Motor 2 Acceleration
        menuNode menu322;

        // Motor 2 Pulse
        menuNode menu323;

    // Relay 1
    menuNode menu33;

    // Relay 2
    menuNode menu34;

// 0-10V Analog
menuNode menu4;

// Units
menuNode menu5;

menuNode menu51;
menuNode menu52;
menuNode menu53;
menuNode menu54;

menuNode userInput;

menuNode disabledCheck;
menuNode enabled;
menuNode enabledCheck;
menuNode disabled;

const uint8_t* getBitmap(const menuNode *name) {
    return name->bitmap;
}

menuNode *getPrev(const menuNode *name) {
    return name->prev;
}

menuNode *getNext(const menuNode *name) {
    return name->next;
}

menuNode *getChild(const menuNode *name) {
    return name->child;
}

menuNode *getParent(const menuNode *name) {
    return name->parent;
}

void buildMenuTree() {
    run.bitmap = runScreen;
    run.prev = NULL;
    run.next = NULL;
    run.child = &menu1;      
    run.parent = NULL;     

    menu1.bitmap = mainMenu1;
    menu1.prev = NULL;
    menu1.next = &menu2;
    menu1.child = NULL;      
    menu1.parent = &run;     

    menu2.bitmap = mainMenu2;
    menu2.prev = &menu1;
    menu2.next = &menu3;
    menu2.child = &menu21;    
    menu2.parent = &run;

    menu3.bitmap = mainMenu3;
    menu3.prev = &menu2;
    menu3.next = &menu4;
    menu3.child = &menu31;    
    menu3.parent = &run;

    menu4.bitmap = mainMenu4;
    menu4.prev = &menu3;
    menu4.next = &menu5;
    menu4.child = &disabledCheck;
    menu4.parent = &run;

    menu5.bitmap = mainMenu5;
    menu5.prev = &menu4;
    menu5.next = NULL;
    menu5.child = &menu51;
    menu5.parent = &run;

    menu21.bitmap = inputs1;
    menu21.prev   = NULL;
    menu21.next   = &menu22;
    menu21.child  = &disabledCheck;
    menu21.parent = &menu2;

    menu22.bitmap = inputs2;
    menu22.prev   = &menu21;
    menu22.next   = &menu23;
    menu22.child  = &disabledCheck;
    menu22.parent = &menu2;

    menu23.bitmap = inputs3;
    menu23.prev   = &menu22;
    menu23.next   = &menu24;
    menu23.child  = &userInput;
    menu23.parent = &menu2;

    menu24.bitmap = inputs4;
    menu24.prev   = &menu23;
    menu24.next   = &menu25;
    menu24.child  = &disabledCheck;
    menu24.parent = &menu2;

    menu25.bitmap = inputs5;
    menu25.prev   = &menu24;
    menu25.next   = &menu26;
    menu25.child  = &userInput;
    menu25.parent = &menu2;

    menu26.bitmap = inputs6;
    menu26.prev   = &menu25;
    menu26.next   = NULL;
    menu26.child  = &userInput;
    menu26.parent = &menu2;

    menu31.bitmap = outputs1;
    menu31.prev   = NULL;
    menu31.next   = &menu32;
    menu31.child  = &menu311;  
    menu31.parent = &menu3;

    menu32.bitmap = outputs2;
    menu32.prev   = &menu31;
    menu32.next   = &menu33;
    menu32.child  = &menu321;  
    menu32.parent = &menu3;

    menu33.bitmap = outputs3;
    menu33.prev   = &menu32;
    menu33.next   = &menu34;
    menu33.child  = &disabledCheck;
    menu33.parent = &menu3;

    menu34.bitmap = outputs4;
    menu34.prev   = &menu33;
    menu34.next   = NULL;
    menu34.child  = &disabledCheck;
    menu34.parent = &menu3;

    menu311.bitmap = motorConfig1;
    menu311.prev   = NULL;
    menu311.next   = &menu312;
    menu311.child  = &userInput;
    menu311.parent = &menu31;

    menu312.bitmap = motorConfig2;
    menu312.prev   = &menu311;
    menu312.next   = &menu313;
    menu312.child  = &userInput;
    menu312.parent = &menu31;

    menu313.bitmap = motorConfig3;
    menu313.prev   = &menu312;
    menu313.next   = NULL;
    menu313.child  = &userInput;
    menu313.parent = &menu31;

    menu321.bitmap = motorConfig1;
    menu321.prev   = NULL;
    menu321.next   = &menu322;
    menu321.child  = &userInput;
    menu321.parent = &menu32;

    menu322.bitmap = motorConfig2;
    menu322.prev   = &menu321;
    menu322.next   = &menu323;
    menu322.child  = &userInput;
    menu322.parent = &menu32;

    menu323.bitmap = motorConfig3;
    menu323.prev   = &menu322;
    menu323.next   = NULL;
    menu323.child  = &userInput;
    menu323.parent = &menu32;

    menu51.bitmap = metricCheckBmp;
    menu51.prev   = NULL;
    menu51.next   = &menu52;
    menu51.child  = NULL;
    menu51.parent = &menu5;

    menu52.bitmap = imperialBmp;
    menu52.prev   = &menu51;
    menu52.next   = NULL;
    menu52.child  = &menu53;
    menu52.parent = &menu5;

    menu53.bitmap = imperialCheckBmp;
    menu53.prev   = &menu54;
    menu53.next   = NULL;
    menu53.child  = NULL;
    menu53.parent = &menu5;

    menu54.bitmap = metricBmp;
    menu54.prev   = NULL;
    menu54.next   = &menu53;
    menu54.child  = &menu51;
    menu54.parent = &menu5;

    disabledCheck.bitmap = disabledCheckBmp;
    disabledCheck.prev   = NULL;
    disabledCheck.next   = &enabled;
    disabledCheck.child  = NULL;
    disabledCheck.parent = NULL;

    enabled.bitmap = enabledBmp;
    enabled.prev   = &disabledCheck;
    enabled.next   = NULL;
    enabled.child  = &enabledCheck;
    enabled.parent = NULL;

    enabledCheck.bitmap = enabledCheckBmp;
    enabledCheck.prev   = &disabled;
    enabledCheck.next   = NULL;
    enabledCheck.child  = NULL;
    enabledCheck.parent = NULL;

    disabled.bitmap = disabledBmp;
    disabled.prev   = NULL;
    disabled.next   = &enabledCheck;
    disabled.child  = &disabledCheck;
    disabled.parent = NULL;

    userInput.bitmap = inputScreen;
    userInput.prev   = NULL;
    userInput.next   = NULL;
    userInput.child  = NULL;
    userInput.parent = NULL;
}

void updateLinkage(menuNode *cur, menuNode *ch) {
    if(ch == &userInput)
        cur->child = ch;
    
    if(ch == &disabledCheck || ch == &enabled || ch == &enabledCheck || ch == &disabled) {
        cur->child = ch;
    }
}


