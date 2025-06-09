
#include "menu.h"

MenuNode run;

MenuNode menu1;
MenuNode menu2;
MenuNode menu3;
MenuNode menu4;
MenuNode menu5;

MenuNode menu21;
MenuNode menu22;
MenuNode menu23;
MenuNode menu24;
MenuNode menu25;
MenuNode menu26;

MenuNode menu211;
MenuNode menu212;
MenuNode menu213;
MenuNode menu214;

MenuNode menu221;
MenuNode menu222;
MenuNode menu223;
MenuNode menu224;

MenuNode menu231;
MenuNode menu232;
MenuNode menu233;
MenuNode menu234;

MenuNode menu241;
MenuNode menu242;
MenuNode menu243;
MenuNode menu244;

MenuNode menu31;
MenuNode menu32;
MenuNode menu33;
MenuNode menu34;

MenuNode menu311;
MenuNode menu312;
MenuNode menu313;

MenuNode menu321;
MenuNode menu322;
MenuNode menu323;

MenuNode menu331;
MenuNode menu332;
MenuNode menu333;
MenuNode menu334;

MenuNode menu341;
MenuNode menu342;
MenuNode menu343;
MenuNode menu344;

MenuNode menu41;
MenuNode menu42;
MenuNode menu43;
MenuNode menu44;

MenuNode menu51;
MenuNode menu52;
MenuNode menu53;
MenuNode menu54;

void buildMenuTree(void) {
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
    menu4.child = &menu41;
    menu4.parent = &run;

    menu5.bitmap = mainMenu5;
    menu5.prev = &menu4;
    menu5.next = NULL;
    menu5.child = &menu51;
    menu5.parent = &run;

    menu21.bitmap = inputs1;
    menu21.prev   = NULL;
    menu21.next   = &menu22;
    menu21.child  = &menu211;
    menu21.parent = &menu2;

    menu211.bitmap = disabledCheck;
    menu211.prev   = NULL;
    menu211.next   = &menu212;
    menu211.child  = NULL;
    menu211.parent = &menu21;

    menu212.bitmap = enabled;
    menu212.prev   = &menu211;
    menu212.next   = NULL;
    menu212.child  = &menu213;
    menu212.parent = &menu21;

    menu213.bitmap = enabledCheck;
    menu213.prev   = &menu214;
    menu213.next   = NULL;
    menu213.child  = NULL;
    menu213.parent = &menu21;

    menu214.bitmap = disabled;
    menu214.prev   = NULL;
    menu214.next   = &menu213;
    menu214.child  = &menu211;
    menu214.parent = &menu21;

    menu22.bitmap = inputs2;
    menu22.prev   = &menu21;
    menu22.next   = &menu23;
    menu22.child  = &menu221;
    menu22.parent = &menu2;

    menu221.bitmap = disabledCheck;
    menu221.prev   = NULL;
    menu221.next   = &menu222;
    menu221.child  = NULL;
    menu221.parent = &menu22;

    menu222.bitmap = enabled;
    menu222.prev   = &menu221;
    menu222.next   = NULL;
    menu222.child  = &menu223;
    menu222.parent = &menu22;

    menu223.bitmap = enabledCheck;
    menu223.prev   = &menu224;
    menu223.next   = NULL;
    menu223.child  = NULL;
    menu223.parent = &menu22;

    menu224.bitmap = disabled;
    menu224.prev   = NULL;
    menu224.next   = &menu223;
    menu224.child  = &menu221;
    menu224.parent = &menu22;

    menu23.bitmap = inputs3;
    menu23.prev   = &menu22;
    menu23.next   = &menu24;
    menu23.child  = NULL;
    menu23.parent = &menu2;

    menu24.bitmap = inputs4;
    menu24.prev   = &menu23;
    menu24.next   = &menu25;
    menu24.child  = &menu241;
    menu24.parent = &menu2;

    menu241.bitmap = disabledCheck;
    menu241.prev   = NULL;
    menu241.next   = &menu242;
    menu241.child  = NULL;
    menu241.parent = &menu24;

    menu242.bitmap = enabled;
    menu242.prev   = &menu241;
    menu242.next   = NULL;
    menu242.child  = &menu243;
    menu242.parent = &menu24;

    menu243.bitmap = enabledCheck;
    menu243.prev   = &menu244;
    menu243.next   = NULL;
    menu243.child  = NULL;
    menu243.parent = &menu24;

    menu244.bitmap = disabled;
    menu244.prev   = NULL;
    menu244.next   = &menu243;
    menu244.child  = &menu241;
    menu244.parent = &menu24;

    menu25.bitmap = inputs5;
    menu25.prev   = &menu24;
    menu25.next   = &menu26;
    menu25.child  = NULL;
    menu25.parent = &menu2;

    menu26.bitmap = inputs6;
    menu26.prev   = &menu25;
    menu26.next   = NULL;
    menu26.child  = NULL;
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
    menu33.child  = &menu331;
    menu33.parent = &menu3;

    menu331.bitmap = disabledCheck;
    menu331.prev   = NULL;
    menu331.next   = &menu332;
    menu331.child  = NULL;
    menu331.parent = &menu33;

    menu332.bitmap = enabled;
    menu332.prev   = &menu331;
    menu332.next   = NULL;
    menu332.child  = &menu333;
    menu332.parent = &menu33;

    menu333.bitmap = enabledCheck;
    menu333.prev   = &menu334;
    menu333.next   = NULL;
    menu333.child  = NULL;
    menu333.parent = &menu33;

    menu334.bitmap = disabled;
    menu334.prev   = NULL;
    menu334.next   = &menu333;
    menu334.child  = &menu331;
    menu334.parent = &menu33;

    menu34.bitmap = outputs4;
    menu34.prev   = &menu33;
    menu34.next   = NULL;
    menu34.child  = &menu341;
    menu34.parent = &menu3;

    menu341.bitmap = disabledCheck;
    menu341.prev   = NULL;
    menu341.next   = &menu342;
    menu341.child  = NULL;
    menu341.parent = &menu34;

    menu342.bitmap = enabled;
    menu342.prev   = &menu341;
    menu342.next   = NULL;
    menu342.child  = &menu343;
    menu342.parent = &menu34;

    menu343.bitmap = enabledCheck;
    menu343.prev   = &menu344;
    menu343.next   = NULL;
    menu343.child  = NULL;
    menu343.parent = &menu34;

    menu344.bitmap = disabled;
    menu344.prev   = NULL;
    menu344.next   = &menu343;
    menu344.child  = &menu341;
    menu344.parent = &menu34;

    menu311.bitmap = motorConfig1;
    menu311.prev   = NULL;
    menu311.next   = &menu312;
    menu311.child  = NULL;
    menu311.parent = &menu31;

    menu312.bitmap = motorConfig2;
    menu312.prev   = &menu311;
    menu312.next   = &menu313;
    menu312.child  = NULL;
    menu312.parent = &menu31;

    menu313.bitmap = motorConfig3;
    menu313.prev   = &menu312;
    menu313.next   = NULL;
    menu313.child  = NULL;
    menu313.parent = &menu31;

    menu321.bitmap = motorConfig1;
    menu321.prev   = NULL;
    menu321.next   = &menu322;
    menu321.child  = NULL;
    menu321.parent = &menu32;

    menu322.bitmap = motorConfig2;
    menu322.prev   = &menu321;
    menu322.next   = &menu323;
    menu322.child  = NULL;
    menu322.parent = &menu32;

    menu323.bitmap = motorConfig3;
    menu323.prev   = &menu322;
    menu323.next   = NULL;
    menu323.child  = NULL;
    menu323.parent = &menu32;

    menu41.bitmap = disabledCheck;
    menu41.prev   = NULL;
    menu41.next   = &menu42;
    menu41.child  = NULL;
    menu41.parent = &menu4;

    menu42.bitmap = enabled;
    menu42.prev   = &menu41;
    menu42.next   = NULL;
    menu42.child  = &menu43;
    menu42.parent = &menu4;

    menu43.bitmap = enabledCheck;
    menu43.prev   = &menu44;
    menu43.next   = NULL;
    menu43.child  = NULL;
    menu43.parent = &menu4;

    menu44.bitmap = disabled;
    menu44.prev   = NULL;
    menu44.next   = &menu43;
    menu44.child  = &menu41;
    menu44.parent = &menu4;

    menu51.bitmap = metricCheck;
    menu51.prev   = NULL;
    menu51.next   = &menu52;
    menu51.child  = NULL;
    menu51.parent = &menu5;

    menu52.bitmap = imperial;
    menu52.prev   = &menu51;
    menu52.next   = NULL;
    menu52.child  = &menu53;
    menu52.parent = &menu5;

    menu53.bitmap = imperialCheck;
    menu53.prev   = &menu54;
    menu53.next   = NULL;
    menu53.child  = NULL;
    menu53.parent = &menu5;

    menu54.bitmap = metric;
    menu54.prev   = NULL;
    menu54.next   = &menu53;
    menu54.child  = &menu51;
    menu54.parent = &menu5;
}