#include "ToneGenerator.h"

void strengthen(Competitor *comp, int unused){
	comp->background_loop_range += STEP;  //maybe add an upper bound
}

void weaken(Competitor* comp, int unused) {
	if(comp->background_loop_range >= STEP) {
		comp->background_loop_range -= STEP;
	}
}

void compete(Competitor* comp, int f) {  //in microsecond
	for(int i = 0;i < comp->background_loop_range;i++);
	SEND(USEC(f), comp->deadline, comp, compete, f);
}

void increase(Tone *tone, int unused){
	if(tone->high <= 15 ){
		tone->high += 1;
	}
}

void decrease(Tone *tone, int unused){
	if(tone->high >= 0x01 ){
		tone->high -= 1;
	}
}

void mute(Tone *tone, int unused){
	tone->mute_value = tone->high;
	tone->high = 0x00;
}

void unmute(Tone *tone, int unused){
	tone->high = tone->mute_value;
}

int getMuteflag(Tone *tone, int unused){
	return tone->mute_flag;
}

void setMuteflag(Tone *tone, int flag){
	tone->mute_flag = flag;
}

void generate_tone(Tone *tone, int f) {
	int half_period = 1000000 / 2 / f;  //in microsecond
	if(*DAC_REGISTER == tone->high) *DAC_REGISTER = tone->low;
	else *DAC_REGISTER = tone->high;
	SEND(USEC(half_period), tone->deadline, tone, generate_tone, f);
}

void setExeFlag(Tone* tone, int flag){
	tone->exeFlag = flag;
}

void generate_tone_nosend(Tone *tone, int f) {
	int half_period = 1000000 / 2 / f;  //in microsecond
	if(*DAC_REGISTER == tone->high) *DAC_REGISTER = tone->low;
	else *DAC_REGISTER = tone->high;
}

void setPeriod(Tone* tone, int period){
	tone->period = period;
}

void enableDeadline(DeadlineController* controller, int unused){
	controller->enableFlag = 1;
}

void disableDeadline(DeadlineController* controller, int unused){
	controller->enableFlag = 0;
}

void setToneDeadline(Tone* tone, int deadline){
	tone->deadline = deadline;
}

void setCompDeadline(Competitor* comp, int deadline){
	comp->deadline = deadline;
}

int getHigh(Tone* tone, int unused){
	return tone->high;
}
