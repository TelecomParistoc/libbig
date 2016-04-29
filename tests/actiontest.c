#include <librobot/robot.h>
#include <librobot/doors.h>
#include <librobot/eater.h>
#include <pathfollower/pathfollower.h>
#include <stdlib.h>
#include <stdio.h>

static void arrivedAtDoor() {
    printf("arrived at doors\n");
    startDoorsAction();
}
static void arrivedAtDoors() {
    startDoorsAction();
}

int main() {
	initRobot();
    setCurrentLocation(129, 1255);
    ffollow("start2cubes", arrivedNearCubes);
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
