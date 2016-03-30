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

/* returns the current table configuration, describing how the shells are placed */
int getTableConfig();

#ifdef __cplusplus
}
#endif

#endif
