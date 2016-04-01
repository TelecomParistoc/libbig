#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/toolboxdriver.h>
#include <stdlib.h>
#include "robot.h"


static void (*gameStartCallback)(void) = NULL;
static void (*collisionDetectCallback)(int) = NULL;
static void (*collisionEndCallback)(int) = NULL;
static int collisions[4] = {0, 0, 0, 0};

static void collisionsCallback() {
    for(int i=0; i<4; i++) {
        if(getCollisionDetector(i+1) && (!collisions[i])) {
            if(collisionDetectCallback != NULL && i > 1)
                collisionDetectCallback(i);
            else if(collisionEndCallback != NULL)
                collisionEndCallback(i);
        } else if((!getCollisionDetector(i+1)) && collisions[i]) {
            if(collisionEndCallback != NULL && i > 1)
                collisionEndCallback(i);
            else if(collisionDetectCallback != NULL)
                collisionDetectCallback(i);
        }
        collisions[i]= getCollisionDetector(i+1);
    }
}
static void sensorsCallback() {
    if(gameStartCallback != NULL && getSensor(1))
        gameStartCallback();
}

void initRobot() {
	initMotionController();
	initToolboxDriver();

	setRobotDistance(0);
	setRobotHeading(0);
	axReset();

	enableSensorCallback(1);
	disableSensorCallback(2);
	disableSensorCallback(3);
	disableSensorCallback(4);
	disableSensorCallback(5);
	setSensorsCallback(sensorsCallback);

	disableCollisionCallback(1);
	disableCollisionCallback(2);
	enableCollisionCallback(3);
	enableCollisionCallback(4);
	enableCollisionCallback(5);
	setCollisionsCallback(collisionsCallback);
}

void onGameStart(void (*callback)(void)) {
    enableSensorCallback(1);
    gameStartCallback = callback;
}

void onCollisionDetect(void (*callback)(int)) {
    collisionDetectCallback = callback;
}
void onCollisionEnd(void (*callback)(int)) {
    collisionEndCallback = callback;
}

int getTableConfig() {
    return getButton(2) + getButton(3)*2 + getButton(1)*3;
}

// roof top buttons
int getTeam() {
    return getButton(5);
}
int getMode() {
    return getButton(4);
}
int getStrategy() {
    return getButton(6) + 1;
}
