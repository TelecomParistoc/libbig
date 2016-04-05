#ifndef EATER_H
#define EATER_H

void inline initDoor();
void inline openDoor();
void inline closeDoor();

void inline setCloseDoorCallback((*callback)(void));
void inline setOpenDoorCallback((*callback)(void));

void startEater();
void stopEater();
#endif
