#ifndef DOORS_H
#define DOORS_H

#ifdef __cplusplus
extern "C" {
#endif

void startDoorsAction();
void pauseDoorsActions();
void resumeDoorsActions();
void onDoorsActionFinished();

#ifdef __cplusplus
}
#endif

#endif
