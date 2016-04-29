#ifndef DOORS_H
#define DOORS_H

#ifdef __cplusplus
extern "C" {
#endif

void startDoorsAction();
void pauseDoorsAction();
void resumeDoorsAction();
int isDoorsActionFinished();

#ifdef __cplusplus
}
#endif

#endif
