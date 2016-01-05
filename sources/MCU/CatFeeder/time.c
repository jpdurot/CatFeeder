/*
* time.c
*
* Created: 21/06/2015 16:08:40
*  Author: JP
*/

#include "time.h"

#include <avr/io.h>
#include <avr/interrupt.h>

time current_time;

alarm definedAlarms[NB_ALARMS];
alarm nextAlarm;
uint8_t hasNextAlarm;

uint8_t areEquals(const time x, const time y)
{
	return (x.hours == y.hours) && (x.minutes == y.minutes) && (x.seconds == y.seconds);
}

// Return -1 if x < y, 0 if x == y and 1 if y > x
int8_t compare(const time x, const time y)
{
	if (x.hours > y.hours)
		return 1;
	if (x.hours < y.hours)
		return -1;
	if (x.minutes > y.minutes)
		return 1;
	if (x.minutes < y.minutes)
		return -1;
	if (x.seconds > y.seconds)
		return 1;
	if (x.seconds < y.seconds)
		return -1;
	
	return 0;
}

void computeNextAlarm()
{	
	if (definedAlarms[0].isActive)
	{
		if (compare(definedAlarms[0].alarm_time, current_time) == 0)
		{
			nextAlarm = definedAlarms[0];
		}
		else if (definedAlarms[1].isActive)
		{			
			hasNextAlarm = 1;
			if ((compare(definedAlarms[0].alarm_time, current_time) == -1) && (compare(definedAlarms[1].alarm_time, current_time) == 1))
			{
				nextAlarm = definedAlarms[1];
			}
			else if ((compare(definedAlarms[1].alarm_time, current_time) == -1) && (compare(definedAlarms[0].alarm_time, current_time) == 1))
			{
				nextAlarm = definedAlarms[0];
			}
			else if (((compare(definedAlarms[1].alarm_time, current_time) == -1) && (compare(definedAlarms[0].alarm_time, current_time) == -1))
			|| ((compare(definedAlarms[1].alarm_time, current_time) == 1) && (compare(definedAlarms[0].alarm_time, current_time) == 1)))
			{
				// Take the lower time				
				if (compare(definedAlarms[0].alarm_time, definedAlarms[1].alarm_time) == -1)
				{
					nextAlarm = definedAlarms[0];
				}
				else
				{
					nextAlarm = definedAlarms[1];
				}
			}
			else if (compare(definedAlarms[1].alarm_time, current_time) == 0)
			{
				nextAlarm = definedAlarms[1];
			}
			else
			{
				nextAlarm = definedAlarms[0];
			}
		}
		else
		{
			nextAlarm = definedAlarms[0];
			hasNextAlarm = 1;
		}
	}
	else if (definedAlarms[1].isActive)
	{
		nextAlarm = definedAlarms[1];
		hasNextAlarm = 1;
	}
	else
	{
		hasNextAlarm = 0;
	}
}

void init_time()
{
	uint8_t hours, minutes, seconds;

	//sscanf(__TIME__, "%d:%d:%d", &hours, &minutes, &seconds);
	hours = 0;
	minutes = 0;
	seconds = 0;
	current_time.hours = hours;
	current_time.minutes = minutes;
	current_time.seconds = seconds;
	TIMSK1 = (1<<OCIE1A);
	TCCR1B = (1<<WGM12)|(1<<CS12);
	TCCR1A = 0;
	OCR1A = 31250;
	sei();
}

//const time get_time()
//{
//return current_time;
//}

void get_time(time* time)
{
	time->hours = current_time.hours;
	time->minutes = current_time.minutes;
	time->seconds = current_time.seconds;
}

void set_time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	current_time.hours = hours;
	current_time.minutes = minutes;
	current_time.seconds = seconds;
	computeNextAlarm();
}

void get_time_string(char* buffer, time* time)
{
	buffer[0] = DIGIT_TO_STRING(time->hours / 10);
	buffer[1] = DIGIT_TO_STRING(time->hours % 10);
	buffer[2] = ':';
	buffer[3] = DIGIT_TO_STRING(time->minutes / 10);
	buffer[4] = DIGIT_TO_STRING(time->minutes % 10);
	buffer[5] = ':';
	buffer[6] = DIGIT_TO_STRING(time->seconds / 10);
	buffer[7] = DIGIT_TO_STRING(time->seconds % 10);
	buffer[8] = 0x00;
}

void set_alarm(uint8_t alarmNumber, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	definedAlarms[alarmNumber].alarm_time.hours = hours;
	definedAlarms[alarmNumber].alarm_time.minutes = minutes;
	definedAlarms[alarmNumber].alarm_time.seconds = seconds;
	definedAlarms[alarmNumber].isActive = 1;
	definedAlarms[alarmNumber].isRaised = 0;
	computeNextAlarm();
}

void disable_alarm(uint8_t alarmNumber)
{
	definedAlarms[alarmNumber].isActive = 0;
	definedAlarms[alarmNumber].isRaised = 0;
	computeNextAlarm();
}

void acknowledge_alarm()
{
	for (int i=0; i < NB_ALARMS; i++)
	{
		definedAlarms[i].isRaised = 0;
	}
	
}

void enable_alarm(uint8_t alarmNumber)
{
	definedAlarms[alarmNumber].isActive = 1;
	definedAlarms[alarmNumber].isRaised = 0;
	computeNextAlarm();
}

void get_alarm(uint8_t alarmNumber, alarm* a)
{
	*a = definedAlarms[alarmNumber];
}

uint8_t isAlarmRaised()
{
	for(int i=0; i < NB_ALARMS; i++)
	{
		if (definedAlarms[i].isActive && definedAlarms[i].isRaised)
		return 1;
	}
	return 0;
}

uint8_t get_nextAlarm(alarm* alarm)
{
	if (hasNextAlarm)
	{
		*alarm = nextAlarm;
		return NEXT_ALARM_OK;
	}
	return NO_NEXT_ALARM;
}

ISR(TIMER1_COMPA_vect)
{
	current_time.seconds++;
	if (current_time.seconds >=60)
	{
		current_time.minutes++;
		current_time.seconds -= 60;
		if (current_time.minutes >=60)
		{
			current_time.hours++;
			current_time.minutes -= 60;
			if (current_time.hours == 24)
			{
				current_time.hours = 0;
			}
		}
	}
	for (int i=0; i < NB_ALARMS; i++)
	{
		if (definedAlarms[i].isActive)
		{
			if (areEquals(definedAlarms[i].alarm_time, current_time))
			{
				definedAlarms[i].isRaised = 1;
				computeNextAlarm();
			}
		}
		
	}
}
