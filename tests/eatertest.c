#include <librobot/robot.h>
#include <librobot/eater.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	initRobot();
	if (atoi(argv[1]))
		startEater();
	else
		stopEater();
	while(1);
	return 0;
}
