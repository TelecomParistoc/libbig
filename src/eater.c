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
#define BRUSHSPEED 500
#define BRUSHON    812
#define BRUSHOFF   280
#define BRUSHMIDDLE 720

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
}

void setBrush() {
	axMove(AXBRUSH, BRUSHON, setBrushCallback);
}
void setBrushMiddle() {
	axMove(AXBRUSH, BRUSHMIDDLE, brushMiddleCallback);
}
void unsetBrush() {
	axMove(AXBRUSH, BRUSHOFF, unsetBrushCallback);
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
	setTargetHeading(0, NULL);
}
static void stopEating() {
	stopEater();
	queueSpeedChange(-0.2, NULL);
	queueStopAt(0, eaterActionFinished);
}

static void stopAndEat();
static void moveAndEat() {
	fastSpeedChange(0.01);
	setSideBlockingCallback(stopAndEat);
}
static void stopAndEat() {
	fastSpeedChange(-0.01);
	setSideBlockingCallback(NULL);
	scheduleIn(500, moveAndEat);
}
static void turnEnd4() {
	fastSpeedChange(0.1);
	queueSpeedChangeAt(80, 0.01, NULL);
	setRobotDistance(0);
	setSideBlockingCallback(stopAndEat);
}
static void backwardFinished(struct motionElement * a) {
	if(a) {}
	setTargetHeading(115, turnEnd4);
}
static void turnEnd3() {
	setRobotDistance(0);
	setBrush();
	queueSpeedChange(-0.1, NULL);
	queueStopAt(-110, backwardFinished);
}

static void backFromCubes(struct motionElement * a) {
	if(a) {}
	setTargetHeading(200, turnEnd3);
}
static void turnEnd2() {
	setRobotDistance(0);
	fastSpeedChange(0);
	queueSpeedChange(-0.2, NULL);
	queueStopAt(-60, backFromCubes);
	setSideBlockingCallback(NULL);
}
static void turnBack2() {
	startEater();
	fastSpeedChange(0);
	setTargetHeading(110, turnEnd2);
	setSideBlockingCallback(turnEnd2);
}
static void turnBack(struct motionElement * a) {
	if(a) {}
	turnBack2();
}
static void turnEnd() {
	setBrushMiddle();
	setRobotDistance(0);
	queueSpeedChange(0.1, NULL);
	queueStopAt(80, turnBack);
	setSideBlockingCallback(turnBack2);
}
// start collecting cubes : first destroy cube stack
void startEaterAction() {
	scheduleIn(20000, stopEating);
	setTargetHeading(130, turnEnd);
	initBrush();
}
