/*
* message.c
*
* Created: 16/07/2015 22:03:43
*  Author: JP
*/

#include "messaging.h"
#include "serial.h"
#include <string.h>

message current_message;
int isAvailable;
uint8_t currentState;

void reset()
{
	isAvailable = 0;
	currentState = MESSAGE_STATE_WAIT_BEGIN;
	current_message.size = 0;
}

void message_init()
{
	serial_init();
	reset();
}

void handleSerialData()
{
	while (serial_dataAvailable() && !isAvailable)
	{
		char byte = serial_read();
		if (byte != '\r' && byte != '\n') {
			current_message.data[current_message.size] = byte;
			current_message.size = current_message.size + 1;
		}
		else /*if ( byte == '\n')*/ {
			if (current_message.size > 0) {
				// End of message
				isAvailable = 1;
			}
		}
	}
}

int message_available()
{
	handleSerialData();
	return isAvailable;
}

void message_get(message* msg)
{
	msg->size = current_message.size;
	memcpy((void*)msg->data, (const void*)current_message.data, current_message.size);
	msg->data[current_message.size] = '\0';
	reset();
}

void message_send(char* data, int dataSize)
{
	char buffer[dataSize+3];
	uint8_t index = 0;
	for (int i=0; i< dataSize;i++)
	{
		buffer[index++] = data[i];
	}
	buffer[index++] = '\r';
	buffer[index++] = '\n';
	buffer[index]= 0; // Mark end of string
	serial_writeString(buffer);
}
