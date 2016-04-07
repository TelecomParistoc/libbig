#ifndef ROBOT_H
#define ROBOT_H

#ifdef __cplusplus
extern "C" {
#endif

/* initialize the robot and make it ready to run. This MUST BE called before any
 * other operations */
void initRobot();

/* calls the given function at the start of the game (when the start jack is
 * unplugged) */
void onGameStart(void (*callback)(void));

/* calls the given function when a collision sensor detects a new obstacle.
 * the callback should be like void myCallback(int collisionSensor) {...}. The
 * argument collisionSensor is the sensor that detected a new obstacle */
void onCollisionDetect(void (*callback)(int));
/* calls the given function when a collision sensor stops detecting an obstacle.
 * the callback should be like void myCallback(int collisionSensor) {...}. The
 * argument collisionSensor is the sensor that stopped detecting an obstacle */
void onCollisionEnd(void (*callback)(int));
/* collision sensors as used in onCollisionDetect/onCollisionEnd : */
#define REAR_LEFT_COLLISION 0
#define REAR_RIGHT_COLLISION 1
#define FRONT_LEFT_COLLISION 2
#define FRONT_RIGHT_COLLISION 3

/* returns the selected team */
int getTeam();
/* teams defined as : */
#define GREEN_TEAM 1
#define PURPLE_TEAM 0

/* returns the selected mode (test or match) */
int getMode();
/* mode defined as : */
#define MATCH_MODE 1
#define TEST_MODE 0

/* returns the selected strategy (1 or 2) */
int getStrategy();

/* returns the current table configuration, describing how the shells are placed
 * returned config is between 0 and 5, 0 being 'OFF' and 1->5 are configurations */
int getTableConfig();

#ifdef __cplusplus
}
#endif

#endif
