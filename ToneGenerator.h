#include "TinyTimber.h"

#define DAC_REGISTER (int*)0x4000741C
#define HIGH 0x05
#define LOW 0x00
#define STEP 500
#define COMPETITOR_DEFAULT 1000
#define COMPETITOR_PERIOD 1300
#define TONEGEN_DDL 100

typedef struct {
	Object super;
	int value;
	int high;
	int low;
	int mute_value;
	int mute_flag;
	int deadline;
	int period;
	int exeFlag;
} Tone;


typedef struct {
	Object super;
	int background_loop_range;
	int deadline;
} Competitor;

typedef struct {
	Object super;
	int deadlineTone;
	int deadlineBackground;
	int enableFlag;
} DeadlineController;

#define initTone(val0, val1) {initObject(), val0, val1, 0x00, 0x05, 1, 0, 1000, 1}
#define initCompetitor(val) {initObject(), val, 0}
#define initDeadlineController(val0, val1, val2) {initObject(), val0, val1, val2} // val0: ToneGenerator Deadline, val1: Background Deadline, val2: Flag

void strengthen(Competitor*, int);
void weaken(Competitor*, int);
void compete(Competitor*, int);
void setCompDeadline(Competitor*, int);

void increase(Tone*, int);
void decrease(Tone*, int);
void mute(Tone*, int);
void unmute(Tone*, int);
void generate_tone(Tone*, int);  //the second parameter: frequency
void generate_tone_nosend(Tone*, int);
void setToneDeadline(Tone*, int);
void setPeriod(Tone*, int);
int getMuteflag(Tone*, int);
void setMuteflag(Tone*, int);
void setExeFlag(Tone*, int);
int getHigh(Tone*, int);

void enableDeadline(DeadlineController*, int);
void disableDeadline(DeadlineController*, int);
