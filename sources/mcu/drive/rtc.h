/*
 * rtc.h
 *
 * Created: 28/06/2015 21:26:56
 *  Author: JP
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "time.h"
#include <avr/io.h>
#include <util/delay.h>

#define PORT_SDA	PORTC
#define PIN_SDA		4
#define PORT_SCL	PORTC
#define PIN_SCL		5

#define SLAVE_ADDRESS 0x68

#define SLAVE_ADDRESS_WRITE (SLAVE_ADDRESS<<1)
#define SLAVE_ADDRESS_READ (SLAVE_ADDRESS<<1)|1

	
#define RTC_REGISTER_TIME_SECONDS				0x00
#define RTC_REGISTER_TIME_MINUTES				0x01
#define RTC_REGISTER_TIME_HOURS					0x02
#define RTC_REGISTER_ALARM1_SECONDS				0x07
#define RTC_REGISTER_ALARM1_MINUTES				0x08
#define RTC_REGISTER_ALARM1_HOURS				0x09
#define RTC_REGISTER_ALARM1_SETTINGS			0x0a
#define RTC_REGISTER_ALARM2_SECONDS				0x0b
#define RTC_REGISTER_ALARM2_MINUTES				0x0c
#define RTC_REGISTER_ALARM2_HOURS				0x0d
#define RTC_REGISTER_CONTROL					0x0e

#define RTC_OK		0
#define RTC_ERROR	1

#define RTC_START_ACK	0x08
#define RTC_RESTART_ACK	0x10
#define RTC_WRITE_ACK	0x28
#define RTC_READ_ACK	0x58 // NACK is returned because we read only 1 byte
#define RTC_SLAVEW_ACK	0x18
#define RTC_SLAVER_ACK	0x40

uint8_t rtc_getTime(time* time);

uint8_t rtc_setTime(time time);

uint8_t rtc_init();

uint8_t rtc_activateAlarm(uint8_t alarmNumber, uint8_t activate);

uint8_t rtc_setAlarm1(time alarm);

uint8_t rtc_getAlarm1(alarm* alarm);

uint8_t rtc_setAlarm2(time alarm);

uint8_t rtc_getAlarm2(alarm* alarm);


// TODO Move in I2C lib
uint8_t readRegister(uint8_t i2cadress, uint8_t address, uint8_t* result);

uint8_t writeRegister(uint8_t i2cadress, uint8_t address, uint8_t value);

#endif /* RTC_H_ */