#include <robotdriver/toolboxdriver.h>
#include <librobot/robot.h>
#include <stdio.h>

/* a simple program to read an AX12 position */

int main() {
    int AX12_ID;
    initRobot();

    printf("Enter AX-12 ID : ");
    scanf("%d", &AX12_ID);
    if(AX12_ID > 254 || AX12_ID < 0) {
        printf("ID must be between 0 and 254\n");
        return -1;
    }

    // no speed, default torque, wheel mode
    axSetTorqueSpeed(AX12_ID, -1, -1, WHEEL_MODE);
    // first read to load position
    axGetPosition(AX12_ID);

    while(1) {
        waitFor(200);
        printf("AX-12 %d at position %d\n", AX12_ID, axGetPosition(AX12_ID));
    }
    return 0;
}
