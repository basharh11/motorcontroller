#ifndef MENU_H
#define MENU_H

#include "bitmaps.h"
#include <stdbool.h>

typedef struct MenuNode {
    const uint8_t *bitmap; // pointer to bitmap to be rendered
    struct MenuNode *prev; // previous item on the same level
    struct MenuNode *next; // next item on the same level
    struct MenuNode *child; // first child (to enter) of this item, or NULL if no child
    struct MenuNode *parent; // parent item (to exit), or NULL if top level
} MenuNode;

extern MenuNode run;

extern MenuNode menu1;
extern MenuNode menu2;
extern MenuNode menu3;
extern MenuNode menu4;
extern MenuNode menu5;

extern MenuNode menu21;
extern MenuNode menu22;
extern MenuNode menu23;
extern MenuNode menu24;
extern MenuNode menu25;
extern MenuNode menu26;

extern MenuNode menu31;
extern MenuNode menu32;
extern MenuNode menu33;
extern MenuNode menu34;

extern MenuNode menu311;
extern MenuNode menu312;
extern MenuNode menu313;

extern MenuNode menu321;
extern MenuNode menu322;
extern MenuNode menu323;

extern MenuNode menu331;
extern MenuNode menu332;
extern MenuNode menu333;
extern MenuNode menu334;

extern MenuNode menu341;
extern MenuNode menu342;
extern MenuNode menu343;
extern MenuNode menu344;

extern MenuNode menu41;
extern MenuNode menu42;
extern MenuNode menu43;
extern MenuNode menu44;

extern MenuNode menu51;
extern MenuNode menu52;
extern MenuNode menu53;
extern MenuNode menu54;

void buildMenuTree(void);

#endif