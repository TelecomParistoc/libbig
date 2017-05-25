#ifndef ROBOT_H
#define ROBOT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "actionsgrobot.h"

// call before game starts
void initRobot();

// call after 90 sec of game
void stopGame();

// call to trigger finish action
void finishAction();

// call actions from input text file
int readAndCall(FILE * file, char c);

#ifdef __cplusplus
}
#endif

#endif // ROBOT_H
