#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motordriver.h>
#include <pathfollower/pathfollower.h>
#include "robot.h"
#include <stdlib.h>
#include <stdio.h>
#include "doors.h"

static int actionState = 0;

int isDoorsActionFinished() {
    return actionState == 4;
}
static void actionFinished() {
    if(actionState >= 4)
        return;
    fastSpeedChange(0);
    setBlockingCallback(NULL);
    setCurrentLocation(446, 1859);
    actionState = 4;
}
static void closeDoor() {
    queueSpeedChange(-0.1, NULL);
    setBlockingCallback(actionFinished);
    scheduleIn(1500, actionFinished);
}
static void turnToDoor(struct motionElement * a) {
    if(a) {}
    actionState=3;
    setActiveDetectors(none);
    setTargetHeading(270, closeDoor);
    setSideBlockingCallback(closeDoor);
}
static void recalibrationEnd() {
    actionState = 2;
    enableHeadingControl(1);
    fastSpeedChange(0);
    setBlockingCallback(NULL);
    setRobotDistance(0);
    setRobotHeading(180);
    setTargetHeading(180, NULL);
    queueSpeedChange(-0.3, NULL);
    queueStopAt(-280, turnToDoor);
}
void resumeDoorsAction() {
    switch (actionState) {
        case 1:
            startDoorsAction();
            break;
        case 2:
    	    queueSpeedChange(-0.3, NULL);
       	    queueStopAt(-260, turnToDoor);
    	    break;
    }
}
void pauseDoorsAction() {
    // stop turning
    setTargetHeading(getRobotHeading(), NULL);
    // stop moving
    clearMotionQueue();
    queueSpeedChange(0, NULL);
}

void startDoorsAction() {
    actionState = 1;
    setActiveDetectors(all);
    /*
    if(getTeam() == GREEN_TEAM)
        setActiveDetectors(right);
    else
        setActiveDetectors(left);
    */
    enableHeadingControl(0);
    queueSpeedChange(0.2, NULL);
    setBlockingCallback(recalibrationEnd);
}
