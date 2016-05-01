#include <librobot/robot.h>
#include <librobot/doors.h>
#include <librobot/eater.h>
#include <librobot/umbrella.h>
#include <pathfollower/pathfollower.h>
#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motioncontroller.h>
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
        scheduleIn(50, quitAndRestart);
    }
	started = 1;
    if(getMode() == TEST_MODE)
        exit(0);
}
static void checkCollisions() {
    static int lastStopped = 0;
    if(getTargetSpeed() >= 0){
        if(isRobotFront()) {
            lastStopped = 1;
            forceStop(1);
        } else if(lastStopped) {
            lastStopped = 0;
            forceStop(0);
        }
    }
    if(getTargetSpeed() <= 0) {
        if(isRobotBehind()) {
            lastStopped = 1;
            forceStop(1);
        } else if(lastStopped) {
            lastStopped = 0;
            forceStop(0);
        }
    }
}
int main() {
    initRobot();
    setCurrentLocation(129, 1255);
    onGameStop(onStop);
    onGameStart(onStart);

    while(started != 1) {
        waitFor(200);
        setLED(1, 0);
        waitFor(200);
        setLED(1, 1);
    }
    while(started != 2)
        waitFor(200);
    setLED(2, 1);
    ffollow("start2cubes", arrivedNearCubes);
    startUmbrellaAction();
    while(!isEaterActionFinished()) {
        waitFor(50);
        checkCollisions();
    }
    printf("finished eater action, going to doors ...\n");
    ffollow("zone2doors", arrivedAtDoors);
    while(!isDoorsActionFinished()) {
        waitFor(50);
        checkCollisions();
    }
    printf("doors closed\n");
    //ffollow("doors2zone", NULL);
	while(1);
	return 0;
}
