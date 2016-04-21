#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/toolboxdriver.h>
#include <stdlib.h>
#include <stdio.h>
#include "robot.h"
#include "umbrella.h"
#include "eater.h"

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
    setRotationDirection(getTeam() == PURPLE_TEAM);

	enableSensorCallback(1);
	disableSensorCallback(2);
	disableSensorCallback(3);
	disableSensorCallback(4);
	disableSensorCallback(5);
	setSensorsCallback(sensorsCallback);

	enableCollisionCallback(1);
	enableCollisionCallback(2);
	enableCollisionCallback(3);
	enableCollisionCallback(4);
	disableCollisionCallback(5);
	setCollisionsCallback(collisionsCallback);
	initUmbrella();
	initBrush();
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

int isRobotFront() {
    printf("check front (positive speed) %d %d %d %d\n",collisions[0],collisions[1],collisions[2],collisions[3]);
    return collisions[1]||collisions[2];
}

int isRobotBehind() {
    printf("check behind (negative speed) %d %d %d %d\n",collisions[0],collisions[1],collisions[2],collisions[3]);
    return collisions[0];
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
    return 2 - getButton(6);
}
