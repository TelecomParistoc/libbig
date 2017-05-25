#include <robotdriver/driver.h>
#include <stdlib.h>

void launchRocket() {
	AX12setTorque(0xFE, 0);
	waitFor(1000);
	AX12setTorque(135, 100);
	AX12move(135, -60, NULL);
}
