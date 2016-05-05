#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
#include <stdlib.h>
#include "umbrella.h"
#include <stdio.h>

#define AXUMBRELLA 144
#define UMBRELLASPEED 100
#define UMBRELLAUP 829
#define UMBRELLADOWN 593

void initUmbrella(){
	axSetTorqueSpeed(AXUMBRELLA, -1, UMBRELLASPEED, 0);
}

void deployUmbrella(){
	axMove(AXUMBRELLA, UMBRELLAUP, NULL, 0);
}

void closeUmbrella(){
	axMove(AXUMBRELLA, UMBRELLADOWN, NULL, 0);
}

static void onEndOfTheGame() {
	initUmbrella();
	deployUmbrella();
	forceStop(1);
	axReset();
	printf("umbrella deploying\n");
}
void startUmbrellaAction() {
	scheduleIn(90000, onEndOfTheGame);
	printf("umbrella scheduled\n");
}
