#ifndef EATER_H
#define EATER_H

void initBrush();
void setBrush();
void unsetBrush();

void setCloseDoorCallback(void (*callback)(void));
void setOpenDoorCallback(void (*callback)(void));

void startEater();
void stopEater();

void startEaterAction();

#endif
