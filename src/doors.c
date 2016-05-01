#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <pathfollower/pathfollower.h>
#include "robot.h"
#include <stdlib.h>
#include <stdio.h>
#include "doors.h"

static int actionState = 0;

int isDoorsActionFinished() {
    return actionState == 7;
}
static void actionFinished() {
    if(actionState >= 7)
        return;
    fastSpeedChange(0);
    setSideBlockingCallback(NULL);
    setCurrentLocation(502, 1781);
    setActiveDetectors(all);
    actionState = 7;
}
static void closeDoor() {
    queueSpeedChange(-0.1, NULL);
    setSideBlockingCallback(actionFinished);
}
static void turnToDoor(struct motionElement * a) {
    if(a) {}
    actionState=6;
    setActiveDetectors(none);
    setTargetHeading(270, closeDoor);
    setSideBlockingCallback(closeDoor);
}
static void moveBackward() {
    actionState=5;
    setRobotDistance(0);
    queueSpeedChange(-0.2, NULL);
    queueStopAt(-100, turnToDoor);
}
static void turnEnd() {
    if(actionState >= 4)
        return;
    actionState=4;
    setSideBlockingCallback(NULL);
    setTargetHeading(180, moveBackward);
}
static void closeFirstDoor(struct motionElement * a) {
    actionState = 3;
    if(a) {}
    setTargetHeading(154, turnEnd);
    setSideBlockingCallback(turnEnd);
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
    queueStopAt(-230, closeFirstDoor);
}
void resumeDoorsAction() {
    switch (actionState) {
        case 1:
            startDoorsAction();
            break;
        case 2:
	    queueSpeedChange(-0.3, NULL);
   	    queueStopAt(-230, closeFirstDoor);
	    break;
	case 3:
	    closeFirstDoor(NULL);
	case 4:
            turnEnd();
            break;
        case 5:
            queueSpeedChange(-0.2, NULL);
            queueStopAt(-190, closeDoor);
            break;
        case 6:
            closeDoor(NULL);
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
    if(getTeam() == GREEN_TEAM)
        setActiveDetectors(right);
    else
        setActiveDetectors(left);

    enableHeadingControl(0);
    queueSpeedChange(0.2, NULL);
    setBlockingCallback(recalibrationEnd);
}
