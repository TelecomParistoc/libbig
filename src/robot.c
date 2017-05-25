#include <robotdriver/driver.h>
#include <stdlib.h>
#include <wiringPi.h>

// call before game starts
void initRobot() {
	wiringPiSetup();
	initAX12(115200);
	initRoof();

	
}

// call after 90 sec of game
void stopGame() {
	stopRobot();
	AX12setTorque(0xFE, 0);
}

// call to trigger finish action
void finishAction() {
	waitFor(1000);
	AX12setTorque(135, 100);
	AX12move(135, -60, NULL);
}
