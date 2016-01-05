/*
 * time.h
 *
 * Created: 21/06/2015 16:08:25
 *  Author: JP
 */ 


#ifndef TIME_H_
#define TIME_H_

#include <avr/common.h>
#include "global.h"

#define NB_ALARMS	2

#define NO_NEXT_ALARM	-1
#define NEXT_ALARM_OK	0


typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} time;

typedef struct 
{
	time alarm_time;
	uint8_t isActive;
	uint8_t isRaised;
}alarm;

void init_time();
void get_time(time* time);
void get_time_string(char* buffer, time* time);
void set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);

void set_alarm(uint8_t alarmNumber, uint8_t hours, uint8_t minutes, uint8_t seconds);
void disable_alarm(uint8_t alarmNumber);
void get_alarm(uint8_t alarmNumber, alarm* a);
void acknowledge_alarm();
void enable_alarm(uint8_t alarmNumber);
uint8_t isAlarmRaised();

uint8_t get_nextAlarm(alarm* alarm);



#endif /* TIME_H_ */