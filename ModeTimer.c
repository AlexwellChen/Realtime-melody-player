#include "TinyTimber.h"
#include "ModeTimer.h"

void setDiff(ModeTimer* mode_timer, long diff){
	mode_timer->diff = diff;
}

long getDiff(ModeTimer* mode_timer, int unused){
	return mode_timer->diff;
}

void setTapTimes(ModeTimer* mode_timer, int tapTimes){
	mode_timer->tapTimes = tapTimes;
}

int getTapTimes(ModeTimer* mode_timer, int unused){
	return mode_timer->tapTimes;
}

void setIsPressed(ModeTimer* mode_timer, int flag){
	mode_timer->isPressed = flag;
}

int getIsPressed(ModeTimer* mode_timer, int unused){
	return mode_timer->isPressed;
}

void setShortDiff(ModeTimer* mode_timer, long shortDiff){
	mode_timer->shortDiff = shortDiff;
}

long getShortDiff(ModeTimer* mode_timer, int unused){
	return mode_timer->shortDiff;
}

void setLongDiff(ModeTimer* mode_timer, long longDiff){
	mode_timer->longDiff = longDiff;
}

long getLongDiff(ModeTimer* mode_timer, int unused){
	return mode_timer->longDiff;
}

Timer *getShortTimer(ModeTimer* mode_timer, int unused){
	return &mode_timer->shortTimer;
}

Timer *getLongTimer(ModeTimer* mode_timer, int unused){
	return &mode_timer->longTimer;
}

Timer *getTimer(ModeTimer* mode_timer, int unused){
	return &mode_timer->timer;
}