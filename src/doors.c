#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <pathfollower/pathfollower.h>
#include <librobot/robot.h>
#include <stdlib.h>
#include <stdio.h>
#include "doors.h"

static void (*doorsCallback)(void) = NULL;
static int actionState = 0;

void onDoorsActionFinished(void (*callback)(void)) {
    doorsCallback = callback;
}
static void actionFinished() {
    setSideBlockingCallback(NULL);
    setCurrentLocation(582, 1793);
    
    if(doorsCallback != NULL)
        doorsCallback();
}
static void closeDoor(struct motionElement * a) {
    if(a) {}
    actionState=4;
    setRobotHeading(210, actionFinished);
    setSideBlockingCallback(actionFinished);
}
static void moveBackward() {
    actionState=3;
    setRobotDistance(0);
    queueSpeedChange(-0.2, NULL);
    queueStopAt(-190, closeDoor);
}
static void turnEnd() {
    actionState=2;
    setSideBlockingCallback(NULL);
    setRobotHeading(180, moveBackward);
}

void resumeDoorsActions() {
    switch (actionState) {
        case 1:
            startDoorsAction();
            break;
        case 2:
            turnEnd();
            break;
        case 3:
            queueSpeedChange(-0.2, NULL);
            queueStopAt(-190, closeDoor);
            break;
        case 4:
            closeDoor();
            break;
    }
}
void pauseDoorsActions() {
    // stop turning
    setTargetHeading(getRobotHeading());
    // stop moving
    clearMotionQueue();
    queueSpeedChange(0, NULL);
}
void startDoorsAction() {
    actionState = 1;
    if(getTeam() == GREEN_TEAM)
        setActiveDetectors(right);
    else
        setActiveDetectors(left);

    setRobotHeading(154, turnEnd);
    setSideBlockingCallback(turnEnd);
}
