#include <librobot/robot.h>
#include <librobot/eater.h>
#include <pathfollower/pathfollower.h>
#include <stdlib.h>

static void arrivedNearCubes() {
    startEaterAction();
}

int main() {
	initRobot();
    setCurrentLocation(129, 1255);
    ffollow("start2cubes", arrivedNearCubes);
	while(1);
	return 0;
}
