#include "TinyTimber.h"

typedef struct {
	Object super;
	Timer timer;  //used to measure the time interval between press and release
	long diff;  //store the time difference between press and release
	int tapTimes;  //first momentary press or second
	int isPressed;  //0 for not pressed, 1 for pressed
	Timer shortTimer;  //PRESS MOMENTARY mode timer
	Timer longTimer;  //PRESS-AND-HOLD mode timer
	long shortDiff;  //PRESS MOMENTARY mode time difference
	long longDiff;  //PRESS-AND-HOLD mode time difference
} ModeTimer;

#define initModeTimer() {initObject(), initTimer(), 0, 0, NOT_PRESSED, initTimer(), initTimer(), 0, 0}

void setDiff(ModeTimer*, long);
long getDiff(ModeTimer*, int);
void setTapTimes(ModeTimer*, int);
int getTapTimes(ModeTimer*, int);
void setIsPressed(ModeTimer*, int);
int getIsPressed(ModeTimer*, int);
void setShortDiff(ModeTimer*, long);
long getShortDiff(ModeTimer*, int);
void setLongDiff(ModeTimer*, long);
long getLongDiff(ModeTimer*, int);
Timer *getShortTimer(ModeTimer*, int);
Timer *getLongTimer(ModeTimer*, int);
Timer *getTimer(ModeTimer*, int);