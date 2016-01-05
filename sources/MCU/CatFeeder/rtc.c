/*
* rtc.c
*
* Created: 28/06/2015 21:27:09
*  Author: JP
*/

#include "rtc.h"
#include "serial.h"
#include "util.h"
#include <util/twi.h>

void waitForResponse()
{
	while (!(TWCR & (1<<TWINT))) {}
	_delay_ms(1);
}

uint8_t rtc_init()
{
	// Init I2C clock
	TWBR = 0x48; // Divide clock by 32 ==> 16MHz/(16 + 2*TWBR) = 100 KHz
	TWSR = TWSR & (~0x03); // No prescaler
	PORT_SDA |= (1<<PIN_SDA); // SDA
	PORT_SCL |= (1<<PIN_SCL); // SCL
	return RTC_OK;
}

uint8_t sendSlaveAddress(uint8_t address)
{
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	waitForResponse();
	
	return TWSR;
}

uint8_t sendByte(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	waitForResponse();
	
	return TWSR;
}

uint8_t readByte(uint8_t* byte)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	waitForResponse();
	uint8_t value = TWDR;
	*byte = value;
	return TWSR;
}

uint8_t sendStart()
{
	// Send START command
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	waitForResponse();
	return TWSR;
}

uint8_t sendStop()
{
	// Send STOP command
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	//waitForResponse();
	return TWSR;
}

uint8_t readRegister(uint8_t i2cadress, uint8_t address, uint8_t* result)
{
	//char buffer[4];
	// ************************
	//serial_writeString("Reading register...");
	//toString(address, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	uint8_t returnValue = 0;
	returnValue = sendStart();
	if (returnValue != RTC_START_ACK && returnValue != RTC_RESTART_ACK)
	return RTC_ERROR;
	// ************************
	//serial_writeString("Start=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	returnValue = sendSlaveAddress(i2cadress<<1);
	if (returnValue != RTC_SLAVEW_ACK)
	return RTC_ERROR;
	// ************************
	//serial_writeString("Send slave address=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	returnValue = sendByte(address);
	if (returnValue != RTC_WRITE_ACK)
	return RTC_ERROR;
	// ************************
	//serial_writeString("Send byte=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	returnValue = sendStart();
	if (returnValue != RTC_RESTART_ACK)
	return RTC_ERROR;
	// ************************
	//serial_writeString("Start=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	returnValue = sendSlaveAddress((i2cadress<<1)|1);
	if (returnValue != RTC_SLAVER_ACK)
	return RTC_ERROR;
	// ************************
	//serial_writeString("Send slave address=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	uint8_t value;
	returnValue = readByte(&value);
	if (returnValue != RTC_READ_ACK)
	return RTC_ERROR;
	// ************************
	//serial_writeString("Read byte=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	//serial_writeString("Value=");
	//toString(value, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	//returnValue = TWSR;
	returnValue = sendStop();
	// ************************
	//serial_writeString("Stop=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	*result = value;
	return RTC_OK;
}

uint8_t writeRegister(uint8_t i2cadress, uint8_t address, uint8_t value)
{
	//char buffer[4];
	// ************************
	//serial_writeString("Writing register...");
	//toString(address, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	uint8_t returnValue = 0;
	returnValue = sendStart();
	// ************************
	//serial_writeString("Start=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	if (returnValue != RTC_START_ACK && returnValue != RTC_RESTART_ACK)
	return RTC_ERROR;
	returnValue = sendSlaveAddress(i2cadress<<1);
	// ************************
	//serial_writeString("Send slave address=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	if (returnValue != RTC_SLAVEW_ACK)
	return RTC_ERROR;
	returnValue = sendByte(address);
	// ************************
	//serial_writeString("Send address=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	if (returnValue != RTC_WRITE_ACK)
	return RTC_ERROR;
	returnValue = sendByte(value);
	// ************************
	//serial_writeString("Send value=");
	//toString(returnValue, buffer);
	//serial_writeString(buffer);
	//serial_writeString("\r\n");
	// ************************
	if (returnValue != RTC_WRITE_ACK)
	return RTC_ERROR;
	sendStop();
	return RTC_OK;
}


uint8_t rtc_getTime(time* time)
{
	uint8_t value;
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_TIME_SECONDS, &value) != RTC_OK)
	return RTC_ERROR;
	
	time->seconds = bcdToInt(value);
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_TIME_MINUTES, &value) != RTC_OK)
	return RTC_ERROR;
	time->minutes = bcdToInt(value);
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_TIME_HOURS, &value) != RTC_OK)
	return RTC_ERROR;
	time->hours = bcdToInt(value);
	return RTC_OK;
}

uint8_t rtc_setTime(time time)
{
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_TIME_HOURS, intToBcd(time.hours)) != RTC_OK)
		return RTC_ERROR;
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_TIME_MINUTES, intToBcd(time.minutes)) != RTC_OK)
		return RTC_ERROR;
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_TIME_SECONDS, intToBcd(time.seconds)) != RTC_OK)
		return RTC_ERROR;
	return RTC_OK;
}

uint8_t rtc_getAlarm1(alarm* alarm)
{
	uint8_t value;
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM1_SECONDS, &value) != RTC_OK)
	return RTC_ERROR;
	
	alarm->alarm_time.seconds = bcdToInt(value);
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM1_MINUTES, &value) != RTC_OK)
	return RTC_ERROR;
	alarm->alarm_time.minutes = bcdToInt(value);
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM1_HOURS, &value) != RTC_OK)
	return RTC_ERROR;
	alarm->alarm_time.hours = bcdToInt(value);
	
	uint8_t control;
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_CONTROL, &control) != RTC_OK)
		return RTC_ERROR;
			
	alarm->isActive = (control & 1); // Alarm 0 is stored on bit 0
	
	return RTC_OK;
}

uint8_t rtc_activateAlarm(uint8_t alarmNumber, uint8_t activate)
{
	if (alarmNumber > 1)
		return RTC_ERROR;
	uint8_t control;
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_CONTROL, &control) != RTC_OK)
		return RTC_ERROR;
	if (activate)
	{
		control |= 1<<alarmNumber; // Bit 0 = Alarm1 Enable
	}
	else
	{
		control &= ~(1<<alarmNumber);
	}
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_CONTROL, control) != RTC_OK)
		return RTC_ERROR;
	return RTC_OK;
}

uint8_t rtc_setAlarm1(time alarm)
{
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM1_HOURS, intToBcd(alarm.hours)) != RTC_OK)
	return RTC_ERROR;
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM1_MINUTES, intToBcd(alarm.minutes)) != RTC_OK)
	return RTC_ERROR;
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM1_SECONDS, intToBcd(alarm.seconds)) != RTC_OK)
	return RTC_ERROR;
	return RTC_OK;
}

uint8_t rtc_getAlarm2(alarm* alarm)
{
	uint8_t value;
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM2_SECONDS, &value) != RTC_OK)
	return RTC_ERROR;
	
	alarm->alarm_time.seconds = bcdToInt(value);
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM2_MINUTES, &value) != RTC_OK)
	return RTC_ERROR;
	alarm->alarm_time.minutes = bcdToInt(value);
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM2_HOURS, &value) != RTC_OK)
	return RTC_ERROR;
	alarm->alarm_time.hours = bcdToInt(value);
	
	uint8_t control;
	if (readRegister(SLAVE_ADDRESS, RTC_REGISTER_CONTROL, &control) != RTC_OK)
	return RTC_ERROR;
	
	alarm->isActive = (control & (1<<1)); // Alarm 1 is stored on bit 0
	
	return RTC_OK;
}

uint8_t rtc_setAlarm2(time alarm)
{
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM2_HOURS, intToBcd(alarm.hours)) != RTC_OK)
	return RTC_ERROR;
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM2_MINUTES, intToBcd(alarm.minutes)) != RTC_OK)
	return RTC_ERROR;
	if (writeRegister(SLAVE_ADDRESS, RTC_REGISTER_ALARM2_SECONDS, intToBcd(alarm.seconds)) != RTC_OK)
	return RTC_ERROR;
	return RTC_OK;
}
