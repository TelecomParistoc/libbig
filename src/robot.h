#ifndef ROBOT_H
#define ROBOT_H

// call before game starts
void initRobot();

// call after 90 sec of game
void stopGame();

// call to trigger finish action
void finishAction();

// in actionhandler.c
int readAndCall(FILE * file, char c);

void

#endif
