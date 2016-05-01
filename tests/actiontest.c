#include <librobot/robot.h>
#include <librobot/doors.h>
#include <librobot/eater.h>
#include <librobot/umbrella.h>
#include <pathfollower/pathfollower.h>
#include <robotdriver/toolboxdriver.h>
#include <stdlib.h>
#include <stdio.h>

int started = 0;
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
static void onStop() {
    if(started == 2) {
        setLED(1, 1);
        setLED(2, 1);
        exit(-23);
    }
	started = 1;
    if(getMode() == TEST_MODE)
        exit(0);
}
int main() {
    initRobot();
    setCurrentLocation(129, 1255);
    onGameStop(onStop);
    onGameStart(onStart);

    while(started != 1) {
        waitFor(200);
        setLED(1, 1);
        waitFor(200);
        setLED(1, 0);
    }
    setLED(1, 0);
    while(started != 2)
        waitFor(200);
    setLED(2, 0);
    ffollow("start2cubes", arrivedNearCubes);
    startUmbrellaAction();
    while(!isEaterActionFinished()) {
        waitFor(100);
    }
    printf("finished eater action, going to doors ...\n");
    ffollow("zone2doors", arrivedAtDoors);
    while(!isDoorsActionFinished()) {
        waitFor(100);
    }
    printf("doors closed\n");
	while(1);
	return 0;
}
