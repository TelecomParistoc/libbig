#include <robotdriver/toolboxdriver.h>
#include <stdlib.h>
#include "umbrella.h"

#define UMBRELLASPEED 50
#define UMBRELLAID 0 //TODO Put good id
#define UMBRELLAUP 812
#define UMBRELLADOWN 564

void inline initUmbrella(){
	axSetTorqueSPEED(UMBRELLAID, -1, UMBRELLASPEED, 0);
}

void inline deployUmbrella(){
	axMove(UMBRELLAID, UMBRELLAUP, NULL);
}

void inline closeUmbrella(){
	axMove(UMBRELLAID, UMBRELLADOWN, NULL);
}
