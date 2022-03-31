#include "TinyTimber.h"

typedef struct {
	Object super;
	int tempo; // T = int(60000000/tempo), in microsecond
	int cnt; // currentId = cnt % 32 (loop)
	int currentId; // select current tone, period, tempo, etc
	int toneFlag; // 0 for tone, 1 for pause, default = 0
	int keyBias; // tone offset, default = 0
	int tonePeriod;
	int T;
	int ifStop;  //0: shouldn't stop    1: stop
	int isSlaveRunning;  //only used in slave mode, 1 for running, 0 for not
	int entity;
	int lightFlag; // light control
} Note;

#define initNote() {initObject(), 120, 0, 0, 0, 0, 1000, 0, 1, 0, 0, 1}

void ctrLength(Note*, int);
void genTone(Note*, int);
void adjustKeyAndTempo(Note*, int);
void initNotePara(Note*, int);
void setNoteKey(Note*, int);
void setNoteTempo(Note*, int);
void setNotePeriod(Note*, int);
int getNotePeriod(Note*, int);
void setNoteKey(Note*, int);
void setNoteTempo(Note*, int);
int getNoteKey(Note*, int);
int getNoteTempo(Note*, int);
void setIfStop(Note*, int);
int getIfStop(Note*, int);
void setEntity(Note*, int);
int getEntity(Note*, int);
void setLightFlag(Note*, int);
void setSlaveRunning(Note*, int);
int getSlaveRunning(Note*, int);
void play(Note*, int);
void stop(Note*, int);
void enableLight(Note*, int);
