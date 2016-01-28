/*
* CatFeeder.c
*
* Created: 21/06/2015 16:05:56
*  Author: JP
*/

#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


#include "time.h"
#include "messaging.h"

#include "serial.h"
#include "rtc.h"

uint8_t lastSecond = 0xFF;
char txBuffer[9];

void displayAlarmInfos(alarm a)
{
	char currentTime[10];
	get_time_string(currentTime, &a.alarm_time);
	serial_writeString("Alarm : ");
	serial_writeString(currentTime);
	if (a.isActive)
	serial_writeString(" - Active");
	else
	serial_writeString(" - Inactive");
	serial_writeString("\r\n");
}

void displayTime(time t)
{
	char currentTime[10];
	get_time_string(currentTime, &t);
	serial_writeString("Time : ");
	serial_writeString(currentTime);
	serial_writeString("\r\n");
}

void displayInfos()
{
	time t;
	get_time(&t);
	displayTime(t);
	alarm a;
	get_alarm(0, &a);
	displayAlarmInfos(a);
	get_alarm(1, &a);
	displayAlarmInfos(a);
	// Next alarm
	if (get_nextAlarm(&a) == NEXT_ALARM_OK)
	{
		char nextAlarm[10];
		get_time_string(nextAlarm, &a.alarm_time);
		serial_writeString("Next alarm : ");
		serial_writeString(nextAlarm);
	}
	else
	{
		serial_writeString("Next alarm : ---");
	}
	serial_writeString("\r\n");
	
	
}

void setup()
{
	
	DDRB |= (1<<PORTB1);
	_delay_ms(4000);
	init_time();
	message_init();
	
	//serial_writeString("Welcome to CatFeeder v0.1\r\n");
	//serial_writeString("-------------------------\r\n");
	rtc_init();

	time current_time;
	rtc_getTime(&current_time);
	alarm a;
	rtc_getAlarm1(&a);
	set_alarm(0, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
	set_time(current_time.hours, current_time.minutes, current_time.seconds);
	rtc_getAlarm2(&a);
	set_alarm(1, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
	//displayInfos();
	
}

typedef void(*messageHandler)(const message msg);

typedef struct 
{
	char id;
	uint8_t expectedSize;
	messageHandler handler;
} messageHandlingInfo;

void handlerGetTime(const message msg)
{
	// Get time command
	char timeBuffer[9];
	time currentTime;
	get_time(&currentTime);
	get_time_string(timeBuffer, &currentTime);
	serial_writeString("Get time : ");
	serial_writeString(timeBuffer);
	serial_writeString("\r\n");
}

void handlerSetTime(const message msg)
{
	char hours = (msg.data[1] - '0') * 10;
	hours = hours + (msg.data[2] - '0');
	char minutes = (msg.data[3] - '0') * 10;
	minutes = minutes + (msg.data[4] - '0');
	char seconds = (msg.data[5] - '0') * 10;
	seconds = seconds + (msg.data[6] - '0');
	time t;
	t.hours = hours;
	t.minutes = minutes;
	t.seconds = seconds;
	rtc_setTime(t);
	time currentTime;
	rtc_getTime(&currentTime);
	set_time(currentTime.hours, currentTime.minutes, currentTime.seconds);
	serial_writeString("Set time OK\r\n");
}

void handlerFeed(const message msg)
{
	if (writeRegister(0x20,0, 0x02) == RTC_OK) // Feed !
	{
		serial_writeString("Miam Kyou & Yoshi !!! \r\n");
	}
}

void handlerPing(const message msg)
{
	serial_writeByte('A');
	serial_writeString("Pong");
	serial_writeByte('Z');
}

#define MESSAGE_HANDLERS_COUNT 4

messageHandlingInfo messageHandlers[MESSAGE_HANDLERS_COUNT] = {
	{ .id = 'T', .expectedSize = 0, .handler = handlerGetTime},
	{ .id = 'S', .expectedSize = 6, .handler = handlerSetTime},
	{ .id = 'F', .expectedSize = 0, .handler = handlerFeed},
	{ .id = 0x47, .expectedSize = 0, .handler = handlerPing}
};



void loop()
{
	time currentTime;
	get_time(&currentTime);
	char timeBuffer[9];
	alarm a;
	if (currentTime.seconds != lastSecond)
	{
		lastSecond = currentTime.seconds;
		if (isAlarmRaised())
		{
			serial_writeString("Feed time !!!\r\n");
			acknowledge_alarm();	
		}
	}
	
	// Handle communication
	if (message_available())
	{
		serial_writeString("Message received\r\n");
		message msg;
		message_get(&msg);
		if (msg.size > 0)
		{
			// Check if a handler exists
			uint8_t isHandled = 0;
			for (int i=0 ; i <MESSAGE_HANDLERS_COUNT;i++)
			{
				if ( messageHandlers[i].id == msg.data[0])
				{
					isHandled = 1;
					// Check expected size
					if (((msg.size -1) == messageHandlers[i].expectedSize) || (messageHandlers[i].expectedSize == 0xff))
					{
						//serial_writeString("Message handled\r\n");
						messageHandlers[i].handler(msg);
					}
					else
					{
						//serial_writeString("Wrong message size\r\n");
					}
					break;
				}
			}
			if (!isHandled)
			{
				//serial_writeString("Unknown action\r\n");
			}
		}

		/*else if (action == 'F')
		{
			
			
		}
		else if (action == '1')
		{
			// Get current alarm
			if (msg.size == 2)
			{
				uint8_t alarmNumber = msg.data[1] - '0';
				get_alarm(alarmNumber, &a);
				get_time_string(timeBuffer, &a.alarm_time);
				serial_writeString("Get alarm : ");
				serial_writeString(timeBuffer);
				serial_writeString("\r\n");
				if (a.isActive)
				{
					serial_writeString("Alarm is active\r\n");
				}
				else
				{
					serial_writeString("Alarm is NOT active\r\n");
				}
			}
			else
			{
				serial_writeString("Get alarm : Wrong message size\r\n");
			}
		}
		else if (action == '2')
		{
			// Set alarm time
			if (msg.size == 8)
			{
				uint8_t alarmNumber = msg.data[1] - '0';
				char hours = (msg.data[2] - '0') * 10;
				hours = hours + (msg.data[3] - '0');
				char minutes = (msg.data[4] - '0') * 10;
				minutes = minutes + (msg.data[5] - '0');
				char seconds = (msg.data[6] - '0') * 10;
				seconds = seconds + (msg.data[7] - '0');
				time t;
				t.hours = hours;
				t.minutes = minutes;
				t.seconds = seconds;
				if (alarmNumber ==0)
				{
					
					if (rtc_setAlarm1(t) != RTC_OK)
					{
						serial_writeString("Set alarm 0 KO !! (0)\r\n");
					}
					else
					{
						if (rtc_getAlarm1(&a) != RTC_OK)
						{
							serial_writeString("Set alarm 0 KO !! (1)\r\n");
						}
						else
						{
							set_alarm(alarmNumber, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
							serial_writeString("Set alarm 0 OK !!!\r\n");
						}
					}
					
				}
				else
				{
					if (rtc_setAlarm2(t) != RTC_OK)
					{
						serial_writeString("Set alarm 1 KO !! (0)\r\n");
					}
					else
					{
						if (rtc_getAlarm2(&a) != RTC_OK)
						{
							serial_writeString("Set alarm 1 KO !! (1)\r\n");
						}
						else
						{
							set_alarm(alarmNumber, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
							serial_writeString("Set alarm 1 OK !!!\r\n");
						}
					}
				}
			}
			else
			{
				serial_writeString("Set time : Wrong message size\r\n");
			}
		}
		else if (action == '3')
		{
			if (msg.size == 2)
			{
				uint8_t alarmNumber = msg.data[1] - '0';
				// Activate alarm
				if (rtc_activateAlarm(alarmNumber, 1) != RTC_OK)
				{
					serial_writeString("Alarm activation KO (0)\r\n");
				}
				else
				{
					if (alarmNumber == 0)
					{
						if (rtc_getAlarm1(&a) != RTC_OK)
						{
							serial_writeString("Alarm activation 0 KO (1)\r\n");
						}
						else
						{
							set_alarm(alarmNumber, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
							if (a.isActive)
							{
								enable_alarm(alarmNumber);
							}
							else
							{
								disable_alarm(alarmNumber);
							}
							serial_writeString("Alarm activation 0 OK !!!\r\n");
						}
					}
					else
					{
						if (rtc_getAlarm2(&a) != RTC_OK)
						{
							serial_writeString("Alarm activation 1 KO (1)\r\n");
						}
						else
						{
							set_alarm(alarmNumber, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
							if (a.isActive)
							{
								enable_alarm(alarmNumber);
							}
							else
							{
								disable_alarm(alarmNumber);
							}
							serial_writeString("Alarm activation 1 OK !!!\r\n");
						}
					}
				}
			}
			else
			{
				serial_writeString("Activate alarm : Wrong message size\r\n");
			}
		}
		else if (action == '4')
		{
			if (msg.size == 2)
			{
				uint8_t alarmNumber = msg.data[1] - '0';
				// Deactivate alarm
				if (rtc_activateAlarm(alarmNumber, 0) != RTC_OK)
				{
					serial_writeString("Alarm deactivation KO (0)\r\n");
				}
				else
				{
					if (alarmNumber == 0)
					{
						if (rtc_getAlarm1(&a) != RTC_OK)
						{
							serial_writeString("Alarm deactivation 0 KO (1)\r\n");
						}
						else
						{
							set_alarm(alarmNumber, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
							if (a.isActive)
							{
								enable_alarm(alarmNumber);
							}
							else
							{
								disable_alarm(alarmNumber);
							}
							serial_writeString("Alarm deactivation 0 OK !!!\r\n");
						}
					}
					else
					{
						if (rtc_getAlarm2(&a) != RTC_OK)
						{
							serial_writeString("Alarm deactivation 1 KO (1)\r\n");
						}
						else
						{
							set_alarm(alarmNumber, a.alarm_time.hours, a.alarm_time.minutes, a.alarm_time.seconds);
							if (a.isActive)
							{
								enable_alarm(alarmNumber);
							}
							else
							{
								disable_alarm(alarmNumber);
							}
							serial_writeString("Alarm deactivation 1 OK !!!\r\n");
						}
					}
				}
			}
			else
			{
				serial_writeString("Deactivate alarm : Wrong message size\r\n");
			}
		}
		else if (action == 'D')
		{
			displayInfos();
		}
		else if (action == '8')
		{
			// Get servo driver status
			uint8_t cmd = '8';
			if (readRegister(0x20,0, &cmd) == RTC_OK)
			{
				serial_writeString("Servo driver command =");
				char str[2];
				str[0] = cmd + '0';
				str[1]= 0;
				serial_writeString(str);
				serial_writeString("\r\n");
			}
			else
			{
				serial_writeString("Servo driver command = ERROR GETTING INFO");
			}
			
			
		}
		else if (action == '9')
		{
			// Get servo driver status
			uint8_t status = '9';
			if (readRegister(0x20,1, &status) == RTC_OK)
			{
				serial_writeString("Servo driver status =");
				char str[2];
				str[0] = status + '0';
				str[1]= 0;
				serial_writeString(str);
				serial_writeString("\r\n");
			}
			else
			{
				serial_writeString("Servo driver status = ERROR GETTING INFO");
			}
			
			
		}
		else
		{
			serial_writeString("Unknown action\r\n");
		}
		*/
	}
}

int main(void)
{
	setup();
	while(1)
	{
		loop();
	}
}