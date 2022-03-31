#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>
#include "ToneGenerator.h"
#include "Note.h"
#include "sioTinyTimber.h"
#include "ModeTimer.h"

/*
 * User guide
 * 
 * The compile flag is consist of 2 digits, first digit represent part number, second digit represent problem number.
 * 
 * To run part 0 problem 2, use #define PROBLEM 02
 * 
 * To run part 0 problem 6, use #define PROBLEM 06
 * 
 * To run part 1 problem 1, use #define PROBLEM 11 "[" decrease "]" increase "m" for mute and unmute
 * 
 * To run part 1 problem 2, use #define PROBLEM 12 "w" increase range, "s" decrease range.
 * 
 * To run part 1 problem 3, use #define PROBLEM 13 "d" for enable/disable deadline.
 * 
 * Part 2 problem 1
 * 
 * access graph: http://assets.processon.com/chart_image/62125f8de401fd587b3b3e75.png
 * timing graph: http://assets.processon.com/chart_image/621267430791297996206963.png
 * 
 * To run part 2 problem 2, use #define PROBLEM 22
 * 
 * To run part 2 problem 3, use #define PROBLEM 23
 * 		To run in leader mode, use #define MODE LEADER.
 * 		To run in slave mode, use #define MODE SLAVE.
 * 
 * To run part 2 problem 4, use #define PROBLEM 24
 * 		For 4.a, 4.b use #define SUB_PROBLEM 1
 * 		For 4.c, 4.d use #define SUB_PROBLEM 2
 * 
 * */
#define PROBLEM 24

#define LEADER 0
#define SLAVE 1

#define MODE LEADER
#define SUB_PROBLEM 2   //only used in part 2 step 4, 1 for 4a and 4b, 2 for 4c

#define NOT_PRESSED 0
#define PRESSED 1

/*
 * For problem 4.d
 */
int frequency_indices[32] = {0,2,4,0,0,2,4,0,4,5,7,4,5,7,7,9,7,5,4,0,7,9,7,5,4,0,0,-5,0,0,-5,0};

/*
 * For problem 5.c
 * 
 * From frequency index [-10, 14]
 * 
 * */
int period[25] = {2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901, 851, 803, 758, 715, 675, 637, 601, 568, 536, 506};

int tune_length[32] = {'a','a','a','a','a','a','a','a','a','a','b','a','a','b','c','c','c','c','a','a','c','c','c','c','a','a','a','a','b','a','a','b'};
/*
 * For lab part1
 * */

typedef struct {
    Object super;
    int count;
    char c;
} App;

typedef struct {
	Object super;
	int buf_index;
	char buf[20];
} Buf;

typedef struct {
	Object super;
	int value[3];
	int val_index;
	int count;
	int buf_index;
	char buf[20];
} Val;

typedef struct{
	Object super;
	Timer timer;
	long diff;
	long max;
	long totalTime;
	int cnt;
} MyTimer;

//used in part 2 step 4 a and b


//used in part 2 step 4 c
typedef struct {
	Object super;
	Timer timer;
	long first;
	long second;
	long third;
	long fourth;
	int index;  //at which tap are we now
} FourTaps;

typedef struct {
	Object super;
	int number;  //how many messages already sent, used to set msgId of CANMsg
	int count;  //how many instructions already given, if reaches 7, no more can be added into ins
	char ins[8];  //instruction to send to slave
} MyMsg;

#define initMyTimer() {initObject(), initTimer(), 0, 0, 0, 0}
#define initModeTimer() {initObject(), initTimer(), 0, 0, NOT_PRESSED, initTimer(), initTimer(), 0, 0}
#define initFourTaps() {initObject(), initTimer(), 0, 0, 0, 0, 0}
#define initMyMsg() {initObject(), 1, 0, {0}}

App app = { initObject(), 0, 'X'};
Buf my_buf = { initObject(), 0, {0}};
Val val = {initObject(), {0}, 0, 0, 0, {0}};
MyTimer my_timer = initMyTimer();
ModeTimer mode_timer = initModeTimer();
FourTaps taps = initFourTaps();
MyMsg my_msg = initMyMsg();

Tone tone = initTone(LOW, 0x05);  //first value should be HIGH
Competitor comp = initCompetitor(COMPETITOR_DEFAULT);
DeadlineController ddlController = initDeadlineController(100, 1300, 0); // val0: ToneGenerator Deadline, val1: Background Deadline, val2: Flag

Note note = initNote();

void reader(App*, int);
void receiver(App*, int);
void handler(App*, int);
void checkIfPressed(ModeTimer*, int);

void three_history(Val*, int);

void convertValue(Buf*, int);
void printPeriod(int);

void calculateWCET(MyTimer*, int);

void adjustVolume(Tone*, int);
void generateTone(Tone *, int);

void adjustCompetitor(Competitor*, int);

void adjustToneDeadline(DeadlineController*, int);
void adjustBackgroundDeadline(DeadlineController*, int);
void adjustDeadline(DeadlineController*, int);

void startNote(Note*, int);
void sendReleaseTip(Note*, int);

void recordIns(CANMsg*, MyMsg*, int);

Timer *getFourTapsTimer(FourTaps *, int);
void setFirstTapTime(FourTaps *, long);
long getFirstTapTime(FourTaps *, int);
void setSecondTapTime(FourTaps *, long);
long getSecondTapTime(FourTaps *, int);
void setThirdTapTime(FourTaps *, long);
long getThirdTapTime(FourTaps *, int);
void setFourthTapTime(FourTaps *, long);
long getFourthTapTime(FourTaps *, int);
void setTapIndex(FourTaps *, int);
int getTapIndex(FourTaps *, int);

//helper function
int isComparable(long, long, long, long);
long absDiff(long, long);
int getMedian(int, int, int);


Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

SysIO sio0 = initSysIO(SIO_PORT0, &app, handler);

// CAN handler
void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
	SCI_WRITE(&sci0, "\n");
	if(MODE == SLAVE){
		for(int i = 0 ; i < 8 ; i++) {
			char c = msg.buff[i];
			if(c == 0) continue;
			if(c == 'z' && SYNC(&note, getIfStop, 0) == 1) ASYNC(&note, play, 0);
			if(c == 'x' && SYNC(&note, getIfStop, 0) == 0) ASYNC(&note, stop, 0);
			ASYNC(&tone, adjustVolume, c);	
			ASYNC(&my_buf, convertValue, c);
		}
	}
}


void reader(App *self, int c) {
	CANMsg cmsg;
    

#if PROBLEM != 23
    SCI_WRITE(&sci0, "Rcv: \'");
    SCI_WRITECHAR(&sci0, c);
    SCI_WRITE(&sci0, "\'\n");
#endif
	
#if PROBLEM == 02
	ASYNC(&val, three_history, c); // part 0 problem 2
	
#elif PROBLEM == 06
	ASYNC(&my_buf, convertValue, c); // part 0 problem 6
	
#elif PROBLEM == 11
	ASYNC(&tone, adjustVolume, c);
	
#elif PROBLEM == 12
	ASYNC(&tone, adjustVolume, c);
	ASYNC(&comp, adjustCompetitor, c);

#elif PROBLEM == 13
	ASYNC(&tone, adjustVolume, c);
	ASYNC(&comp, adjustCompetitor, c);
	ASYNC(&ddlController, adjustDeadline, c);
	
#elif PROBLEM == 22
	ASYNC(&tone, adjustVolume, c);	
	ASYNC(&my_buf, convertValue, c);
	
#elif PROBLEM == 23
	if(MODE == LEADER) {
		if(c == 'z' && SYNC(&note, getIfStop, 0) == 1){ 
			play(&note, 0);
		}else if(c == 'x' && SYNC(&note, getIfStop, 0) == 0){
			stop(&note, 0);
			
		}
		recordIns(&cmsg, &my_msg, c);
		ASYNC(&my_buf, convertValue, c);
		ASYNC(&tone, adjustVolume, c);
	} else if(MODE == SLAVE) {
		if(getSlaveRunning(&note, 0) == 0) {
			if(c == 'z') {
				SCI_WRITE(&sci0, "Enter Slave!\n");
				setSlaveRunning(&note, 1);
//				note.ifStop = 0;
//				ASYNC(&note, setIfStop, 0); // 0 is running, 1 is stop
				SYNC(&note, startNote, 0);
			}
			
		} else {
			recordIns(&cmsg, &my_msg, c);
			if(c != 'l') {
				SCI_WRITECHAR(&sci0, c);
			} else {
				SCI_WRITECHAR(&sci0, '\n');
			}
		}
	}
#elif PROBLEM == 24
if(c == 'z' && SYNC(&note, getIfStop, 0) == 1){ 
	play(&note, 0);
	ASYNC(&note, enableLight, 0);
	}else if(c == 'x' && SYNC(&note, getIfStop, 0) == 0){
		stop(&note, 0);
	}
	
	ASYNC(&my_buf, convertValue, c);
	ASYNC(&tone, adjustVolume, c);
#endif
}

void startApp(App *self, int arg) {
    CANMsg msg;
    CAN_INIT(&can0);
    SCI_INIT(&sci0);
	SIO_INIT(&sio0);
    SCI_WRITE(&sci0, "Hello, hello...\n");
	
#if PROBLEM == 11
	ASYNC(&tone, generate_tone, 1000);
#elif PROBLEM == 12
	ASYNC(&tone, generate_tone, 769);
	ASYNC(&comp, compete, COMPETITOR_PERIOD);
#elif PROBLEM == 13
	ASYNC(&tone, generate_tone, 1000);
	ASYNC(&comp, compete, COMPETITOR_PERIOD);
#elif PROBLEM == 14
	ASYNC(&my_timer, calculateWCET, 0);
#elif PROBLEM == 22
	SCI_WRITE(&sci0, "'t' & 'y' for volume, 'm' for mute & unmute, number for key & tempo, 'p' to confirm vaule\n");
	ASYNC(&note, startNote, 0);
#elif PROBLEM == 23
	SCI_WRITE(&sci0,"Slave mode: first press 'z' to enter slave, control command is the same as Leader mode. After type command, press 'l' to send CAN message. \nLeader mode: 'z' for start, 'x' for stop. 'l' to send CAN message.\n");
	SCI_WRITE(&sci0, "'t' & 'y' for volume, 'm' for mute & unmute, number for key & tempo, 'p' to confirm vaule\n");
	ASYNC(&note, startNote, 0);
#elif PROBLEM == 24
	SCI_WRITE(&sci0, "'z' to start melody, 'x' to stop melody, 't' & 'y' for volume, 'm' for mute & unmute, number for key & tempo, 'p' to confirm vaule\n");
	SIO_WRITE(&sio0, 1);
	ASYNC(&note, startNote, 0);
#endif

}

/*
 * Code for Part 2 step 3: Problem 3.a, 3.b
 * */
void recordIns(CANMsg *cmsg, MyMsg *my_msg, int c) {  //record instructions to be sent to CAN
	if(c == 'l') {  //press 'l' to send instruction to CAN
		cmsg->msgId = my_msg->number;
		cmsg->nodeId = 1;
		cmsg->length = 8;
		cmsg->buff[0] = my_msg->ins[0];
		cmsg->buff[1] = my_msg->ins[1];
		cmsg->buff[2] = my_msg->ins[2];
		cmsg->buff[3] = my_msg->ins[3];
		cmsg->buff[4] = my_msg->ins[4];
		cmsg->buff[5] = my_msg->ins[5];
		cmsg->buff[6] = my_msg->ins[6];
		cmsg->buff[7] = 0;
		CAN_SEND(&can0, cmsg);
		my_msg->number++;
		my_msg->ins[0] = 0;
		my_msg->ins[1] = 0;
		my_msg->ins[2] = 0;
		my_msg->ins[3] = 0;
		my_msg->ins[4] = 0;
		my_msg->ins[5] = 0;
		my_msg->ins[6] = 0;
		my_msg->count = 0;
	} else if(my_msg->count <= 7) {
		my_msg->count++;
		my_msg->ins[my_msg->count - 1] = c;
	}
}

/* 
 * Problem 2 
 * 
 * Create by ytdd
 * 
 * */
#if PROBLEM == 02
void three_history(Val *val, int c) {
	switch(c) {
		case 'F':
			SCI_WRITE(&sci0, "The 3-history has been erased\n");
			val->buf_index = 0;
			val->val_index = 0;
			val->count = 0;
			for(int i = 0;i < 20;i++) {
				val->buf[i] = 0;
			}
			break;
		case 'e':
			{
				if(val->buf_index == 0) {
					SCI_WRITE(&sci0, "No input found\n");
					return;
				}
				if(val->count < 3) val->count++;
				int sum = 0, med = 0;
				char output[100];
				val->buf[val->buf_index++] = '\0';
				val->value[val->val_index] = atoi(val->buf);
				for(int i = 0;i < val->count;i++) {
					sum += val->value[i];
				}
				if(val->count == 1) {
					med = val->value[0];
				} else if(val->count == 2) {
					med = (val->value[0] + val->value[1]) / 2;
				} else if(val->count == 3) {
					med = getMedian(val->value[0], val->value[1], val->value[2]);
				}
				snprintf(output, 100, "Entered integer %d: sum = %d, median = %d\n",val->value[val->val_index], sum, med);
				SCI_WRITE(&sci0, output);
				val->buf_index = 0; // reset the convert buffer index
				val->val_index = (val->val_index + 1) % 3; // get next value store index
				for(int i = 0;i < 20;i++) {
					val->buf[i] = 0; // reset convert buffer
				}
			}
			break;
		case '0':
			val->buf[val->buf_index++] = '0';
			break;
		case '1':
			val->buf[val->buf_index++] = '1';
			break;
		case '2':
			val->buf[val->buf_index++] = '2';
			break;
		case '3':
			val->buf[val->buf_index++] = '3';
			break;
		case '4':
			val->buf[val->buf_index++] = '4';
			break;
		case '5':
			val->buf[val->buf_index++] = '5';
			break;
		case '6':
			val->buf[val->buf_index++] = '6';
			break;
		case '7':
			val->buf[val->buf_index++] = '7';
			break;
		case '8':
			val->buf[val->buf_index++] = '8';
			break;
		case '9':
			val->buf[val->buf_index++] = '9';
			break;
		case '-':
			if(val->buf_index != 0) SCI_WRITE(&sci0, "\'-\' can only be at the beginning of a number\n");
			else val->buf[val->buf_index++] = '-';
			break;
		default:
			break;
	}
}

int getMedian(int a, int b, int c) {
	int med = a;
	if(a > b && a > c) {
		if(b > c) med = b;
		else med = c;
	} else if(a < b && a < c) {
		if(b < c) med = b;
		else med = c;
	}
	return med;
}

/*
 * Problem 3
 * 
 * 3.a: b
 * 
 * 3.b: 500
 * 
 * */
 
 /*
  * Problem 4
  * 
  * 4.a: 1136us
  * 
  * 4.b: 2^{k/12}
  * 
  * 4.c: (a) 1/(2*440Hz*2^(-7/12)) = 1703us (b) 1/(2*440Hz*2^(9/12)) = 676us
  * 
  * 4.e: Maximum is 9, Minimum is -5
  * 
  * */
  
  /*
   * Problem 5
   * 
   * 5.a: -5, -3, -1, -5, -5, -3, -1, -5, -1, 0
   * 
   * 5.b: 14, -10
   * 
   * 5.d: x(k) = k + 10 
   * */

/*
 * Problem 6
 * 
 * Create by Alexwell
 * 
 * */
#endif
void convertValue(Buf *my_buf, int c){
	switch(c){
		case 'p':
			if(my_buf->buf_index == 0) {
				SCI_WRITE(&sci0, "No input found\n");
				return;
			}
			my_buf->buf[my_buf->buf_index++] = '\0';
			int value = atoi(my_buf->buf);
			my_buf->buf_index = 0;
			for(int i = 0;i < 20;i++) {
				my_buf->buf[i] = 0;
			}
			
//			printPeriod(value);
			char output[50];
			snprintf(output, 50, "Value:%d\n", value);
			SCI_WRITE(&sci0, output);
			ASYNC(&note, adjustKeyAndTempo, value);
			
			break;
		case '0':
			my_buf->buf[my_buf->buf_index++] = '0';
			break;
		case '1':
			my_buf->buf[my_buf->buf_index++] = '1';
			break;
		case '2':
			my_buf->buf[my_buf->buf_index++] = '2';
			break;
		case '3':
			my_buf->buf[my_buf->buf_index++] = '3';
			break;
		case '4':
			my_buf->buf[my_buf->buf_index++] = '4';
			break;
		case '5':
			my_buf->buf[my_buf->buf_index++] = '5';
			break;
		case '6':
			my_buf->buf[my_buf->buf_index++] = '6';
			break;
		case '7':
			my_buf->buf[my_buf->buf_index++] = '7';
			break;
		case '8':
			my_buf->buf[my_buf->buf_index++] = '8';
			break;
		case '9':
			my_buf->buf[my_buf->buf_index++] = '9';
			break;
		case '-':
			if(my_buf->buf_index != 0) SCI_WRITE(&sci0, "\'-\' can only be at the beginning of a number\n");
			else my_buf->buf[my_buf->buf_index++] = '-';
			break;
		default:
			break;
	}
}


void printPeriod(int key){
	if(key < -5 || key > 5){
		SCI_WRITE(&sci0, "invalid key!\n");
		return;
	}
	char output[20];
	snprintf(output, 20, "Key: %d\n",key);
	SCI_WRITE(&sci0, output);	

	for(int i = 0; i < 32; i++){
		snprintf(output, 20, "%d", period[frequency_indices[i] + key + 10]);
		SCI_WRITE(&sci0, output);
		SCI_WRITE(&sci0, " ");
	}
	SCI_WRITE(&sci0, "\n");
}


void adjustVolume(Tone *tone, int c){
	char output[100];
	switch(c){
		case 'y':
//			ASYNC(tone, increase, 0);
			increase(tone, 0);
			snprintf(output, 100, "current high value:%d", tone->high);
			SCI_WRITE(&sci0, output);
			SCI_WRITE(&sci0, "\n");
			break;
		case 't':
//			ASYNC(tone, decrease, 0);
			decrease(tone, 0);
			snprintf(output, 100, "current high value:%d", tone->high);
			SCI_WRITE(&sci0, output);
			SCI_WRITE(&sci0, "\n");
			break;
		case 'm':
			if(tone->mute_flag == 1){
//				ASYNC(tone, mute, 0);
				mute(tone, 0);
//				ASYNC(tone, setMuteflag, 0);
				setMuteflag(tone, 0);
			}else{
//				ASYNC(tone, unmute, 0);
				unmute(tone, 0);
//				ASYNC(tone, setMuteflag, 1);
				setMuteflag(tone, 1);
			}
			snprintf(output, 100, "current high value:%d", tone->high);
			SCI_WRITE(&sci0, output);
			SCI_WRITE(&sci0, "\n");
			break;
		default:
			break;
	}
}

void adjustCompetitor(Competitor* comp, int c) {
	char output[100];
	switch(c) {
		case 'w':
			ASYNC(comp, strengthen, 0);
			snprintf(output, 100, "Current value of background_loop_range: %d\n", comp->background_loop_range);
			SCI_WRITE(&sci0, output);
			break;
		case 's':
			ASYNC(comp, weaken, 0);
			snprintf(output, 100, "Current value of background_loop_range: %d\n", comp->background_loop_range);
			SCI_WRITE(&sci0, output);
			break;
		default:
			break;
	}
}

void adjustToneDeadline(DeadlineController* controller, int deadline){
	ASYNC(&tone, setToneDeadline, deadline);
	controller->deadlineTone = deadline;
}

void adjustBackgroundDeadline(DeadlineController* controller, int deadline){
	ASYNC(&comp, setCompDeadline, deadline);
	controller->deadlineBackground = deadline;
}

void adjustDeadline(DeadlineController* controller, int c){
	switch(c){
		case 'd':
			if(controller->enableFlag == 1){
				ASYNC(controller, disableDeadline, 0);
				ASYNC(controller, adjustToneDeadline, 0);
				ASYNC(controller, adjustBackgroundDeadline, 0);
				SCI_WRITE(&sci0, "Disable Deadline\n");
			}else {
				ASYNC(controller, enableDeadline, 0);
				ASYNC(controller, adjustToneDeadline, 100);
				ASYNC(controller, adjustBackgroundDeadline, 1300);
				SCI_WRITE(&sci0, "Enable Deadline\n");
			}
		default:
			break;
	}
}

// Problem 3.b backgroud occupy the sound, 1KHz tone getting smaller and smaller.
// Problem 3.c 13500

void calculateWCET(MyTimer* my_timer, int unused){
	char output[50];
	
	if(my_timer->cnt < 500){
		//T_RESET(&my_timer->timer);
		my_timer->diff = USEC_OF(CURRENT_OFFSET());
		
		for(int i = 0;i < 13500;i++);  // Problem 4.a and 4.b, for 4.b, just change the loop range
//		for(int i = 0;i < 1000;i++){    //Problem 4.c
//			generate_tone_nosend(&tone, 1000);
//			int half_period = 1000000 / 2 / 1000;  //in microsecond
//			if(*DAC_REGISTER == tone.high) *DAC_REGISTER = tone.low;
//			else *DAC_REGISTER = tone.high;
//		}
		my_timer->diff = USEC_OF(CURRENT_OFFSET()) - my_timer->diff;
		if(my_timer->diff > my_timer->max){
			my_timer->max = my_timer->diff;
		}
		my_timer->totalTime += my_timer->diff;
		my_timer->cnt += 1;
		SEND(USEC(1000), 0, my_timer, calculateWCET, 0);
	}else{
		my_timer->totalTime = my_timer->totalTime / 500;
		snprintf(output, 50, "Max WCET: %ld, Average WCET:%ld\n", my_timer->max, my_timer->totalTime);
		SCI_WRITE(&sci0, output);
	}
}
/////////////////////////////////////////////////////////////////////////
//--------------------Code below is for Part 2-------------------------//
/////////////////////////////////////////////////////////////////////////
/*
 * Code for Part 2 step 2
 * */
void startNote(Note* note, int unused){
	//init parameter
	initNotePara(note, 0);
#if PROBLEM == 22
	setIfStop(note, 0);
#endif
	// When lightflag equal to -1 means tempo has changed, light needs to be synchronized.
	if(note->lightFlag == -1){
		setLightFlag(note, 1);
		AFTER(USEC(note->tempo), note, enableLight, unused);
	}
	SYNC(&tone, setPeriod, period[frequency_indices[note->currentId] + note->keyBias + 10]);
	SYNC(&tone, setExeFlag, 1);
	ASYNC(&tone, generateTone, 0);

	char tuneLength = tune_length[note->currentId];
	int a = (int)60000000/(note->tempo);  //1000000 / (tempo / 60)
	switch(tuneLength){
		case 'a':
			note->T = a;
			break;
		case 'b':
			note->T = 2 * a;
			break;
		case 'c':
			note->T = (int)a/2;
			break;
		default:
			break;
	}
	
	
	if(note->ifStop == 0) {
		AFTER(USEC(note->T), note, startNote, unused); // Set next note task
		int pauseStart = 0.9 * note->T;
		AFTER(USEC(pauseStart), note, ctrLength, unused); // Setup pause task
	}
//	AFTER(USEC(note->T), note, startNote, unused); // Set next note task
//	int pauseStart = 0.9 * note->T;
//	AFTER(USEC(pauseStart), note, ctrLength, unused); // Setup pause task
	
}

void generateTone(Tone *tone, int unused) {
	int half_period = tone->period;  //in microsecond
	if(*DAC_REGISTER == tone->high) *DAC_REGISTER = tone->low;
	else *DAC_REGISTER = tone->high;
	if(tone->exeFlag == 1 && getIfStop(&note, 0) == 0){  //exeFlag controls a single tone, ifStop controls the whole tune
		SEND(USEC(half_period), tone->deadline, tone, generateTone, unused);
	}
}

void ctrLength(Note* note, int unused){
	ASYNC(&tone, setExeFlag, 0);
	SIO_WRITE(&sio0, 1);
}

void adjustKeyAndTempo(Note* note, int value){
	if(value < 6 && value > -6){
		SCI_WRITE(&sci0, "Key set!\n");
//		ASYNC(note, setNoteKey, value);
		note->keyBias = value;
	}else if(value <=240 && value >=60){
//		ASYNC(note, setNoteTempo, value);
		note->tempo = value;
		SCI_WRITE(&sci0, "Tempo set!\n");
	}else{
		SCI_WRITE(&sci0, "invalid value, tempo value should between [60, 240], key value should between [-5, 5]\n");
	}
}

/*
 * Code for Part 2 step 4
 * */

// Code for Problem 4.a, 4.b, 4.c
void handler(App *self, int unused) {

	if(getIsPressed(&mode_timer, 0) == NOT_PRESSED) {
		setIsPressed(&mode_timer, PRESSED);
		SIO_TRIG(&sio0, 1);  //set trigger event to be press-to-release
		long diff = T_SAMPLE(getTimer(&mode_timer, 0));
		setDiff(&mode_timer, diff);
#if SUB_PROBLEM == 1
		AFTER(MSEC(1000), &mode_timer, checkIfPressed, 0);
#elif SUB_PROBLEM == 2
//		AFTER(MSEC(2000), &note, sendReleaseTip, 0);
		
#endif
	} else {
		long diff = (T_SAMPLE(getTimer(&mode_timer, 0)) - getDiff(&mode_timer, 0)) / 100;
		setDiff(&mode_timer, diff);
		if(diff < 1000) {  //momentary press
#if SUB_PROBLEM == 1
			SCI_WRITE(&sci0, "PRESS-MOMENTARY mode\n");
			if(getTapTimes(&mode_timer, 0) == 0) {  //first tap
				T_RESET(getShortTimer(&mode_timer, 0));
				setTapTimes(&mode_timer, 1);
				long shortDiff = T_SAMPLE(getShortTimer(&mode_timer, 0));
				setShortDiff(&mode_timer, shortDiff);
			} else if(getTapTimes(&mode_timer, 0) == 1) {  //second tap
				long temp = (T_SAMPLE(getShortTimer(&mode_timer, 0)) - getShortDiff(&mode_timer, 0)) / 100;
				if(temp < 100) {
					SCI_WRITE(&sci0, "Contact bounce!\n");
				} else {
					setTapTimes(&mode_timer, 0);
					char output[100];
					snprintf(output, 100, "time difference of two taps is: %ldms\n", temp);
					SCI_WRITE(&sci0, output);
				}
			}
#elif SUB_PROBLEM == 2
			long first, second, third, fourth;
			switch(getTapIndex(&taps, 0)) {
				case 0:
					T_RESET(getFourTapsTimer(&taps, 0));
					first = T_SAMPLE(getFourTapsTimer(&taps, 0)) / 100;
					setFirstTapTime(&taps, first);
					setTapIndex(&taps, getTapIndex(&taps, 0) + 1);
					SCI_WRITE(&sci0, "first tap\n");
					break;
				case 1:
					second = T_SAMPLE(getFourTapsTimer(&taps, 0)) / 100;
					setSecondTapTime(&taps, second);
					setTapIndex(&taps, getTapIndex(&taps, 0) + 1);
					SCI_WRITE(&sci0, "second tap\n");
					break;
				case 2:
					third = T_SAMPLE(getFourTapsTimer(&taps, 0)) / 100;
					setThirdTapTime(&taps, third);
					setTapIndex(&taps, getTapIndex(&taps, 0) + 1);
					SCI_WRITE(&sci0, "third tap\n");
					break;
				case 3:
					fourth = T_SAMPLE(getFourTapsTimer(&taps, 0)) / 100;
//					setFourthTapTime(&taps, fourth);
					SYNC(&taps, setFourthTapTime, fourth);
//					setTapIndex(&taps, 0);
					SYNC(&taps, setTapIndex, 0);
					SCI_WRITE(&sci0, "fourth tap\n");
					first = getFirstTapTime(&taps, 0);
					second = getSecondTapTime(&taps, 0);
					third = getThirdTapTime(&taps, 0);
					
					if(isComparable(first, second, third, fourth) == 1) {
						long temp = (absDiff(first, second)\
											+ absDiff(second, third)\
											+ absDiff(third, fourth)) / 3;
						if(temp >= 200 && temp <= 2000) {  //[30, 300] 200ms->300bpm, 2000ms ->30bpm
							temp = 60000 / temp;
							setNoteTempo(&note, temp);
							ASYNC(&note, setLightFlag, -1);// light synchrony flag
							char output[50];
							snprintf(output, 50, "the tempo will be set to: %ldbpm\n", temp);
							SCI_WRITE(&sci0, output);
						} else {
							SCI_WRITE(&sci0, "out of tempo range\n");
						}
					} else {
						SCI_WRITE(&sci0, "not comparable, please retry\n\n");
					}
					break;
				default:
					break;
			}
#endif
		} 
#if SUB_PROBLEM == 2
		else if(getDiff(&mode_timer, 0) > 2000){
			SCI_WRITE(&sci0, "temp reset to 120bpm\n");
			setTapIndex(&taps, 0);
			setFirstTapTime(&taps, 0);
			setSecondTapTime(&taps, 0);
			setThirdTapTime(&taps, 0);
			setFourthTapTime(&taps, 0);
			setNoteTempo(&note, 120);
			ASYNC(&note, setLightFlag, -1); // light synchrony flag
//			setLightFlag(&note, 1);
//			enableLight(&note, 0);
		}
#endif
		else{  //press-and-hold
#if SUB_PROBLEM == 1
			long longDiff = T_SAMPLE(getLongTimer(&mode_timer, 0)) / 100000 + 1;  //?
			setLongDiff(&mode_timer, longDiff);
			SCI_WRITE(&sci0, "leaving PRESS-AND-HOLD mode\n");
			char output[100];
			snprintf(output, 100, "Your staying time is: %lds\n", longDiff);
			SCI_WRITE(&sci0, output);
#endif
		}
		setIsPressed(&mode_timer, NOT_PRESSED);
		SIO_TRIG(&sio0, 0);  //restore default trigger event
	}
}

void sendReleaseTip(Note *note, int unused){
	SCI_WRITE(&sci0, "Release to reset!\n");
}

// Code for problem 4.a 4.b 4.c
void checkIfPressed(ModeTimer *mode_timer, int unused) {
	int status = SIO_READ(&sio0);
	if(status == 0) {  //still pressed
#if SUB_PROBLEM == 1
		T_RESET(&mode_timer->longTimer);
		SCI_WRITE(&sci0, "entering PRESS-AND-HOLD mode\n");
		AFTER(MSEC(1000), &note, sendReleaseTip, 0);
		mode_timer->longDiff = T_SAMPLE(&mode_timer->longTimer);
#elif SUB_PROBLEM == 2
		SCI_WRITE(&sci0, "temp reset to 120bpm\n");
		setTapIndex(&taps, 0);
		setFirstTapTime(&taps, 0);
		setSecondTapTime(&taps, 0);
		setThirdTapTime(&taps, 0);
		setFourthTapTime(&taps, 0);
		setNoteTempo(&note, 120);
		setLightFlag(&note, 1);
		
#endif
	}
}

long absDiff(long a, long b) {
	return a > b ? a - b : b - a;
} 

int isComparable(long a, long b, long c, long d) {
	long e = absDiff(a, b), f = absDiff(b, c), g = absDiff(c, d);
	long p[3] = {e, f, g};
	long max = e, min = e;
	for(int i = 1 ; i < 3 ; i++) {
		if(p[i] < min) min = p[i];
		if(p[i] > max) max = p[i];
	}
	if(max - min > 250) return 0;  //not comparable
	else return 1;  //comparable
} 

void play(Note *note, int unused) {
//		reinit note
	note->lightFlag = 1;
	note->tempo = 120;
	note->cnt = 0;
	note->currentId = 0;
	note->toneFlag = 0;
	note->keyBias = 0;
	note->tonePeriod = 1000;
	note->T = 0;
	note->ifStop = 0;
	ASYNC(note, startNote, 0);
}

void stop(Note *note, int unused){
	if(note->ifStop == 0) {
		note->ifStop = 1;
	} 
	note->lightFlag = 0;
}

void enableLight(Note* note, int unused){
	int a = (int)60000000/(note->tempo);
	AFTER(USEC(0), &sio0, sio_write, 0);
	AFTER(USEC(a/2), &sio0, sio_write, 1);
	if(note->lightFlag == 1){
		AFTER(USEC(a), note, enableLight, 0);
	}
}

Timer *getFourTapsTimer(FourTaps *taps, int unused){
	return &taps->timer;
}

void setFirstTapTime(FourTaps *taps, long time){
	taps->first = time;
}

long getFirstTapTime(FourTaps *taps, int unused){
	return taps->first;
}

void setSecondTapTime(FourTaps *taps, long time){
	taps->second = time;
}

long getSecondTapTime(FourTaps *taps, int unused){
	return taps->second;
}

void setThirdTapTime(FourTaps *taps, long time){
	taps->third = time;
}

long getThirdTapTime(FourTaps *taps, int unused){
	return taps->third;
}

void setFourthTapTime(FourTaps *taps, long time){
	taps->fourth = time;
}

long getFourthTapTime(FourTaps *taps, int unused){
	return taps->fourth;
}

void setTapIndex(FourTaps *taps, int index){
	taps->index = index;
}

int getTapIndex(FourTaps *taps, int unused){
	return taps->index;
}

int main() {
    INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
	INSTALL(&can0, can_interrupt, CAN_IRQ0);
	INSTALL(&sio0, sio_interrupt, SIO_IRQ0);
    TINYTIMBER(&app, startApp, 0);
    return 0;
}
