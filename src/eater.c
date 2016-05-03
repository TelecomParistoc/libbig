#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/toolboxdriver.h>
#include <pathfollower/pathfollower.h>
#include <stdlib.h>
#include <stdio.h>
#include "robot.h"
#include "eater.h"

// Ax-12 that controls the brushes
// middle : rayon 235.5mm
#define AXBRUSH 133
#define BRUSHSPEED 500
#define BRUSHON 812
#define BRUSHOFF 280
#define BRUSHMIDDLE 720

#define AXDOOR 141
#define DOORSPEED 400
#define AXDOORCLOSED 670
#define AXDOOROPEN 248

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
static void (*brushMiddleCallback)(void) = NULL;

void initBrush() {
	axSetTorqueSpeed(AXBRUSH, -1, BRUSHSPEED, 0);
	axSetTorqueSpeed(AXDOOR, -1, DOORSPEED, 0);
}

void setBrush() {
	axMove(AXBRUSH, BRUSHON, setBrushCallback, 0);
}
void setBrushMiddle() {
	axMove(AXBRUSH, BRUSHMIDDLE, brushMiddleCallback, 0);
}
void unsetBrush() {
	axMove(AXBRUSH, BRUSHOFF, unsetBrushCallback, 0);
}

void setSetBrushCallback(void (*callback)(void)){
	setBrushCallback = callback;
}
void setBrushMiddleCallback(void (*callback)(void)){
	brushMiddleCallback = callback;
}
void setUnsetBrushCallback(void (*callback)(void)){
	unsetBrushCallback = callback;
}

void openDoor() {
	axMove(AXDOOR, AXDOOROPEN, NULL, 0);
}
void closeDoor() {
	axMove(AXDOOR, AXDOORCLOSED, NULL, 0);
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

// ############## action moves : ##############
static int actionState = 0;

static void cornerFromCenter();
static void robotEmpty() {
	//actionState = 10;
	stopEater();
	closeDoor();
	unsetBrush();
	setActiveDetectors(all);
	setSideBlockingCallback(NULL);
	if(actionState == 8) {
		setCurrentLocation(1180, 1386);
		actionState = 11;
	} else {
		setCurrentLocation(1300, 1386);
		ffollow("zone2cubes", cornerFromCenter);
	}
}
static void unloadCubes() {
	fastSpeedChange(0);
	setBlockingCallback(NULL);
	setRobotHeading(90);
	enableHeadingControl(1);
	startEater();
	openDoor();
	if(actionState == 8)
		scheduleIn(7000, robotEmpty);
	else
		scheduleIn(3000, robotEmpty);
}
static void turnEnd5() {
	setActiveDetectors(none);
	queueSpeedChange(-0.1, NULL);
	enableHeadingControl(0);
	setBlockingCallback(unloadCubes);
}
static void nearZone(struct motionElement * a) {
	//actionState = 9;
	if(a) {}
	printf("finished eating\n");
	setTargetHeading(90,turnEnd5);
}
static void stopEating() {
	actionState = 8;
	enableHeadingControl(1);
	setActiveDetectors(rear);
	stopEater();
	queueSpeedChange(-0.2, NULL);
	queueStopAt(-210, nearZone);
	setSideBlockingCallback(NULL);
}

static double lastDistanceR = -20000;
static double lastDistanceL = -20000;

static void speedManager();
static void goForwardToCubes() {
	fastSpeedChange(0.01);
	scheduleIn(200, speedManager);
}
static void speedManager() {
	if(actionState >= 7 || lastDistanceR == -20000)
		return;
	double differenceR = getRdistance()-lastDistanceR;
	double differenceL = getLdistance()-lastDistanceL;
	lastDistanceL = getLdistance();
	lastDistanceR = getRdistance();
	differenceR = differenceR > 0 ? differenceR : 0;
	differenceL = differenceL > 0 ? differenceL : 0;
	if((differenceL <= 0.4 || differenceR <= 0.4) && getTargetSpeed() == 0.01) {
		fastSpeedChange(-0.01);
		scheduleIn(300, goForwardToCubes);
	} else {
		fastSpeedChange(0.01);
		scheduleIn(200, speedManager);
	}
	scheduleIn(200, speedManager);
}
static void turnEnd4() {
	setActiveDetectors(none);
	queueSpeedChange(0.1, NULL);
	queueSpeedChangeAt(65, 0.01, NULL);
	setRobotDistance(0);
	enableHeadingControl(0);
	scheduleIn(1000, speedManager);
	lastDistanceR = -20000;
	lastDistanceL = -20000;
	startEater();
}
static void backwardFinished(struct motionElement * a) {
	actionState = 7;
	if(a) {}
	setTargetHeading(115, turnEnd4);
}
static void turnEnd3() {
	actionState = 6;
	setRobotDistance(0);
	setBrush();
	queueSpeedChange(-0.1, NULL);
	queueStopAt(-120, backwardFinished);

}

static void backFromCubes(struct motionElement * a) {
	actionState = 5;
	if(a) {}
	setTargetHeading(200, turnEnd3);

}
static void turnEnd2() {
	actionState = 4;
	setRobotDistance(0);
	fastSpeedChange(0);
	queueSpeedChange(-0.2, NULL);
	queueStopAt(-60, backFromCubes);
	setSideBlockingCallback(NULL);
	setMaxAcceleration(0.5);
}
static void turnBack() {
	actionState = 3;
	fastSpeedChange(0);
	setMaxAcceleration(0.1);
	setTargetHeading(120, turnEnd2);
	//setSideBlockingCallback(turnEnd2);
}
static void turnEnd() {
	actionState = 2;
	setBrushMiddle();
	queueSpeedChange(0.1, NULL);
	setSideBlockingCallback(turnBack);
}
// start collecting cubes : first destroy cube stack
static void eatCorner(struct motionElement * a) {
	actionState = 1;
	if(a) {}
	scheduleIn(24000, stopEating);
	setTargetHeading(150, turnEnd);
	initBrush();
	setActiveDetectors(rear);
}

static void cornerFromCenter() {
	eatCorner(NULL);
}
static void stopEatingCenter() {
	actionState = 18;
	enableHeadingControl(1);
	setActiveDetectors(rear);
	stopEater();
	queueSpeedChange(-0.2, NULL);
	queueStopAt(-190, nearZone);
	setSideBlockingCallback(NULL);
}
static void turnEnd01() {
	queueSpeedChange(0.2, NULL);
	queueSpeedChangeAt(60, 0.01, NULL);
	setRobotDistance(0);
	enableHeadingControl(0);
	scheduleIn(1200, speedManager);
	startEater();
}
static void moveToCenter(struct motionElement * a) {
	actionState = 17;
	if(a) {}
	scheduleIn(8000, stopEatingCenter);
	setActiveDetectors(none);
	setBrush();
	setTargetHeading(73, turnEnd01);
}
static void checkFreeWay() {
	if(isRobotFront()) {
		clearMotionQueue();
		queueSpeedChange(-0.3, NULL);
		queueStopAt(0, eatCorner);
	} else if(actionState >= 17) {
		scheduleIn(50, checkFreeWay);
		setActiveDetectors(all);
	}
}
static void turnEnd0() {
	scheduleIn(50, checkFreeWay);
	setRobotDistance(0);
	queueSpeedChange(0.3, NULL);
	queueStopAt(388, moveToCenter);
}
void startEaterAction() {
	actionState = 0;
	if(getStrategy() == 1) {
		eatCorner(NULL);
	} else {
		setTargetHeading(0, turnEnd0);
		initBrush();
		setActiveDetectors(front);
	}
}

void pauseEaterAction() {
	// stop turning
    setTargetHeading(getRobotHeading(), NULL);
    // stop moving
    clearMotionQueue();
    queueSpeedChange(0, NULL);
}
void resumeEaterAction() {
	switch (actionState) {
		case 1:
			setTargetHeading(150, turnEnd);
			break;
		case 2:
			turnEnd();
			break;
		case 3:
			turnBack();
			break;
		case 4:
			queueSpeedChange(-0.2, NULL);
			queueStopAt(-60, backFromCubes);
			break;
		case 5:
			backFromCubes(NULL);
			break;
		case 6:
			queueSpeedChange(-0.1, NULL);
			queueStopAt(-120, backwardFinished);
			break;
		case 7:
			backwardFinished(NULL);
			break;
		case 8:
			stopEating();
			break;
		case 9:
			nearZone(NULL);
			break;
	}
}
int isEaterActionFinished() {
	return actionState == 11;
}
