/*
 * util.h
 *
 * Created: 09/09/2015 21:45:01
 *  Author: JP
 */


#ifndef UTIL_H_
#define UTIL_H_


void toString(uint8_t value, char* string)
{
	uint8_t index = 0;
	uint8_t hundreds = value / 100;
	if (hundreds > 0 )
	{
		string[index++] = hundreds + '0';
	}
	uint8_t tens = (value - hundreds*100) / 10;
	if (tens > 0 ||index == 1)
	{
		string[index++] = tens + '0';
	}
	uint8_t units = (value - hundreds*100 - tens*10);
    string[index++] = units + '0';
	string[index] = 0; // NULL
}

uint8_t bcdToInt(uint8_t value)
{
	return 10* (value>>4 & 0x0f) + (value & 0x0f);
}

uint8_t intToBcd(uint8_t value)
{
	return ((value/10) <<4) + (value%10);
}



#endif /* UTIL_H_ */
