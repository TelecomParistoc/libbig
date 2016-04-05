#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/toolboxdriver.h>
#include <stdlib.h>
#include "eater.h"

//TODO Set ids

// Ax-12 that controls the door

#define AXDOOR     0
#define DOORSPEED  100
#define DOOROPEN   802
#define DOORCLOSED 262

// Ax-12 that controls the conveyor belt
//TODO Check rotation direction

#define AXCONVEYOR    0
#define CONVEYORSPEED 1023

// Ax-12 that control the brushes
// TODO Check rotation direction

#define AXLEFTBRUSH  0
#define AXRIGHTBRUSH 0
#define AXLEFTSPEED  1023
#define AXRIGHTSPEED (-1023)

static void (*openDoorCallback)(void) = NULL;
static void (*closeDoorCallback)(void) = NULL;

void inline initDoor(){
	axSetTorqueSpeed(AXDOOR, -1, DOORSPEED, 0);
}

void inline openDoor(){
	axMove(AXDOOR, DOOROPEN, openDoorCallback);
}

void inline closeDoor(){
	axMove(AXDOOR, DOORCLOSED, closeDoorCallback);
}

void inline setCloseDoorCallback((*callback)(void));
	closeDoorCallback = callback;
}

void inline setOpenDoorCallback((*callback)(void));
	openDoorCallback = callback;
}

void startEater(){
	axSetTorqueSpeed(AXCONVEYOR,   -1, CONVEYORSPEED,   1);
	axSetTorqueSpeed(AXLEFTBRUSH,  -1, LEFTBRUSHSPEED,  1);
	axSetTorqueSpeed(AXRIGHTBRUSH, -1, RIGHTBRUSHSPEED, 1);
}

void stopEater(){
	axSetTorqueSpeed(AXCONVEYOR,   -1, 0, 1);
	axSetTorqueSpeed(AXLEFTBRUSH,  -1, 0, 1);
	axSetTorqueSpeed(AXRIGHTBRUSH, -1, 0, 1);
}
