#include <librobot/robot.h>
#include <librobot/doors.h>
#include <pathfollower/pathfollower.h>
#include <stdlib.h>
#include <stdio.h>

static void arrivedAtDoor() {
    startDoorsAction();
}
static void onEnd() {
    printf("doors closed !\n");
}
int main() {
	initRobot();
    setCurrentLocation(1162, 1386);
    onDoorsActionFinished(onEnd);
    ffollow("zone2doors", arrivedAtDoor);
	while(1);
	return 0;
}
