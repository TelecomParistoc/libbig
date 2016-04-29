#ifndef EATER_H
#define EATER_H

#ifdef __cplusplus
extern "C" {
#endif

void initBrush();
void setBrush();
void unsetBrush();

void setCloseDoorCallback(void (*callback)(void));
void setOpenDoorCallback(void (*callback)(void));

void startEater();
void stopEater();

void startEaterAction();

#ifdef __cplusplus
}
#endif

#endif
