#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/toolboxdriver.h>
#include <stdlib.h>
#include <stdio.h>
#include "eater.h"

// Ax-12 that controls the brushes

#define AXBRUSH    133
#define BRUSHSPEED 100
#define BRUSHON    802
#define BRUSHOFF   262

// Ax-12 that controls the conveyor belt
//TODO Check rotation direction

#define AXCONVEYOR    135
#define CONVEYORSPEED (-1023)

// Ax-12 that control the brushes
// TODO Check rotation direction

#define AXLEFTBRUSH  143
#define AXRIGHTBRUSH 142
#define AXLEFTSPEED  1023
#define AXRIGHTSPEED (-1023)

static void (*setBrushCallback)(void) = NULL;
static void (*unsetBrushCallback)(void) = NULL;

void initBrush(){
	axSetTorqueSpeed(AXBRUSH, -1, BRUSHSPEED, 0);
}

void setBrush(){
	axMove(AXBRUSH, BRUSHON, setBrushCallback);
}

void unsetBrush(){
	axMove(AXBRUSH, BRUSHOFF, unsetBrushCallback);
}

void setSetBrushCallback(void (*callback)(void)){
	setBrushCallback = callback;
}

void setUnsetBrushCallback(void (*callback)(void)){
	unsetBrushCallback = callback;
}

void startEater(){
	axSetTorqueSpeed(AXCONVEYOR,   -1, CONVEYORSPEED, 1);
	axSetTorqueSpeed(AXLEFTBRUSH,  -1, AXLEFTSPEED,   1);
	axSetTorqueSpeed(AXRIGHTBRUSH, -1, AXRIGHTSPEED,  1);
}

void stopEater(){
	axSetTorqueSpeed(AXCONVEYOR,   -1, 0, 1);
	axSetTorqueSpeed(AXLEFTBRUSH,  -1, 0, 1);
	axSetTorqueSpeed(AXRIGHTBRUSH, -1, 0, 1);
}

static void eaterActionFinished(struct motionElement * a) {
	if(a) {}
	printf("finished eating\n");
}

// stop collecting cubes and move backward to the place where the action started
static void stopEating() {
	stopEater();
	// move backward
	queueSpeedChange(-0.1, NULL);
	setTargetHeading(135, NULL);
	queueStopAt(0, eaterActionFinished);
}

static void onCubeBlocking();
// try to move forward again
static void onCubeRestart() {
	static angle = 15;
	queueSpeedChange(0.05, NULL);
	turnOf(angle);
	angle = -angle;
	setBlockingCallback(onCubeBlocking);
	printf("resume moving\n");
}
// avoid forcing when hitting a cube
static void onCubeBlocking() {
	scheduleIn(700, onCubeRestart);
	setBlockingCallback(NULL);
	fastSpeedChange(-0.05);
}
// then go and eat cubes !
static void goForCubes() {
	setBlockingCallback(onCubeBlocking);
	queueSpeedChange(0.1, NULL);
	// set up the timeout
	scheduleIn(10000, stopEating);
	startEater();
}
// start collecting cubes : first turn toward cubes
void startEaterAction() {
	setTargetHeading(135, goForCubes);
}
