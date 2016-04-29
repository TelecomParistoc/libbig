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
static void (*gameStopCallback)(void) = NULL;
static void (*collisionDetectCallback)(int) = NULL;
static void (*collisionEndCallback)(int) = NULL;
static int collisions[4] = {0, 0, 0, 0};
static int lastJack = 0;
static collisionConfig_t collConfig = all;

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
    if(gameStartCallback != NULL && getSensor(1) && !lastJack) {
        gameStartCallback();
        lastJack = 1;
    } else if(gameStopCallback != NULL && !getSensor(1) && lastJack){
        gameStopCallback();
        lastJack = 0;
    }
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

    lastJack = getSensor(1);
}

void onGameStart(void (*callback)(void)) {
    enableSensorCallback(1);
    gameStartCallback = callback;
}
void onGameStop(void (*callback)(void)) {
    enableSensorCallback(1);
    gameStopCallback = callback;
}

void onCollisionDetect(void (*callback)(int)) {
    collisionDetectCallback = callback;
}
void onCollisionEnd(void (*callback)(int)) {
    collisionEndCallback = callback;
}
void setActiveDetectors(collisionConfig_t config) {
    collConfig = config;
}

int isRobotFront() {
    printf("check front (positive speed) %d %d %d %d\n",collisions[0],collisions[1],collisions[2],collisions[3]);
    switch (collConfig) {
        case all:
        case front:
            return collisions[FRONT_RIGHT_COLLISION] || collisions[FRONT_LEFT_COLLISION];
        case left:
            return collisions[FRONT_LEFT_COLLISION];
        case right:
            return collisions[FRONT_RIGHT_COLLISION];
        case rear:
        case none:
            return 0;
    }
    return 0;
}

int isRobotBehind() {
    printf("check behind (negative speed) %d %d %d %d\n",collisions[0],collisions[1],collisions[2],collisions[3]);
    switch (collConfig) {
        case all:
        case rear:
            return (!collisions[REAR_RIGHT_COLLISION]) || (!collisions[REAR_LEFT_COLLISION]);
        case left:
            return (!collisions[REAR_LEFT_COLLISION]);
        case right:
            return (!collisions[REAR_RIGHT_COLLISION]);
        case front:
        case none:
            return 0;
    }
    return 0;
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
