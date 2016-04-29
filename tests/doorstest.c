#include <librobot/robot.h>
#include <librobot/doors.h>
#include <pathfollower/pathfollower.h>
#include <stdlib.h>
#include <stdio.h>

static void arrivedAtDoor() {
    startDoorsAction();
}

int main() {
	initRobot();
    setCurrentLocation(1162, 1386);
    ffollow("zone2doors", arrivedAtDoor);
	while(!isDoorsActionFinished()) {
        waitFor(100);
    }
    printf("door action finished\n");
    while(1);
	return 0;
}
