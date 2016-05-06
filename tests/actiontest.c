#include <librobot/robot.h>
#include <librobot/doors.h>
#include <librobot/eater.h>
#include <librobot/umbrella.h>
#include <pathfollower/pathfollower.h>
#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/headingcontroller.h>
#include <robotdriver/motordriver.h>
#include <stdlib.h>
#include <stdio.h>

static int started = 0;

static void arrivedAtDoors() {
    printf("arrived at doors\n");
    startDoorsAction();
}
static void arrivedNearCubes() {
    startEaterAction();
}
static void onStart() {
	if(started == 1)
		started = 2;
}
static void quitAndRestart() {
    exit(-23);
}
static void onStop() {
    if(started == 2) {
        setLED(1, 0);
        setLED(2, 0);
        setLED(3, 0);
        scheduleIn(50, quitAndRestart);
    }
	started = 1;
    if(getMode() == TEST_MODE)
        exit(0);
}
static void checkCollisions() {
    static int lastStopped = 0;
    if(getTargetSpeed() > 0){
        if(isRobotFront()) {
            lastStopped = 1;
            forceStop(1);
            setLED(3, 1);
        } else if(lastStopped) {
            lastStopped = 0;
            forceStop(0);
            setLED(3, 0);
        }
    }
    if(getTargetSpeed() < 0) {
        if(isRobotBehind()) {
            lastStopped = 1;
            forceStop(1);
            setLED(3, 1);
        } else if(lastStopped) {
            lastStopped = 0;
            forceStop(0);
            setLED(3, 0);
        }
    }
}
static void enableAllSensors() {
	setActiveDetectors(all);
    printf("sensors activated\n");
}
static void enableSideSensors() {
    if(getTeam() == GREEN_TEAM)
        setActiveDetectors(right);
    else
        setActiveDetectors(left);
}
static void stopRobot() {
    forceStop(1);
}

int main() {
    // init
    initRobot();
    enableHeadingControl(0);
    setCurrentLocation(129, 1255);
    onGameStop(onStop);
    onGameStart(onStart);
    setLED(3, 0);
    setLED(2, 0);

    // wait for start
    while(started != 1) {
    	waitFor(200);
    	setLED(1, 0);
    	waitFor(200);
    	setLED(1, 1);
    }
    axReset();
    while(started != 2)
    	waitFor(200);

    // start
    setLED(2, 1);
    setLED(1, 0);
    setRobotHeading(0);
    setTargetHeading(0, NULL);
    setRobotDistance(0);
    enableHeadingControl(1);
    setActiveDetectors(all);

    //cubes
    ffollow("start2cubes", arrivedNearCubes);
    startUmbrellaAction();
    while(!isEaterActionFinished()) {
    	waitFor(50);
    	checkCollisions();
    }
    printf("finished eater action, going to doors ...\n");

    // doors
    enableSideSensors();
	ffollow("zone2doors", arrivedAtDoors);
    while(!isDoorsActionFinished()) {
    	waitFor(50);
    	checkCollisions();
    }
    printf("doors closed\n");

    // seashells
    setActiveDetectors(none);
    forceStop(0);
    scheduleIn(5000, enableAllSensors);
    ffollow("doors2zone", stopRobot);

    while(1) {
        waitFor(50);
    	checkCollisions();
    }
    return 0;
}
