#include "TinyTimber.h"
#include "ToneGenerator.h"
#include "Note.h"

void setNotePeriod(Note* note, int value){
	note->T = value;
}

int getNotePeriod(Note* note, int value){
	return note->T;
}

void setNoteKey(Note* note, int value){
	note->keyBias = value;
}

void setNoteTempo(Note* note, int value){
	note->tempo = value;
}

int getNoteKey(Note* note, int unused){
	return note->keyBias;
}

int getNoteTempo(Note* note, int unused){
	return note->tempo;
}

void initNotePara(Note* note, int unused){
	note->currentId = note->cnt % 32;
	note->cnt += 1;
	note->toneFlag = 0;
}

void setIfStop(Note* note, int ifStopFlag){
	note->ifStop = ifStopFlag;
}

int getIfStop(Note* note, int unused){
	return note->ifStop;
}

void setEntity(Note* note, int num){
	note->entity += num;
}
int getEntity(Note* note, int unused){
	return note->entity;
}

void setLightFlag(Note* note, int flag){
	note->lightFlag = flag;
}

void setSlaveRunning(Note* note, int flag){
	note->isSlaveRunning = flag;
}

int getSlaveRunning(Note* note, int unused){
	return note->isSlaveRunning;
}