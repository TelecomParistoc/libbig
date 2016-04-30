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
    return actionState == 5;
}
static void actionFinished() {
    if(actionState >= 5)
        return;
    setSideBlockingCallback(NULL);
    setCurrentLocation(582, 1781);
    actionState = 5;
}
static void closeDoor(struct motionElement * a) {
    if(a) {}
    actionState=4;
    setTargetHeading(210, actionFinished);
    setSideBlockingCallback(actionFinished);
}
static void moveBackward() {
    actionState=3;
    setRobotDistance(0);
    queueSpeedChange(-0.2, NULL);
    queueStopAt(-190, closeDoor);
}
static void turnEnd() {
    if(actionState >= 2)
        return;
    actionState=2;
    setSideBlockingCallback(NULL);
    setTargetHeading(180, moveBackward);
}
static void closeFirstDoor(struct motionElement * a) {
    if(a) {}
    setTargetHeading(154, turnEnd);
    setSideBlockingCallback(turnEnd);
}
static void recalibrationEnd() {
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
            turnEnd();
            break;
        case 3:
            queueSpeedChange(-0.2, NULL);
            queueStopAt(-190, closeDoor);
            break;
        case 4:
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
