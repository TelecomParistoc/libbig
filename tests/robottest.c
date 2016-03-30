#include <robotdriver/toolboxdriver.h>
#include <librobot/robot.h>
#include <stdio.h>

static void collisionDetect(int sensor) {
    printf("Obstacle detected from sensor %d\n", sensor);
}
static void collisionEnd(int sensor) {
    printf("No more obstacle in front of sensor %d\n", sensor);
}
static void gameStart() {
    printf("game start !\n");
}

int main () {
    initRobot();

    onCollisionDetect(collisionDetect);
    onCollisionEnd(collisionEnd);

    onGameStart(gameStart);
    
    printf("Current config : %d\n", getTableConfig());

    while(1);
    return 0;
}
