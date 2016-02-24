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
		/*serial_writeString("\r\n");serial_writeString("\r\n");
		serial_writeString("Current byte = ");
		serial_writeByte(byte);
		serial_writeString("\r\n");
		
		serial_writeString("Current state = ");
		serial_writeByte(currentState + '0');
		serial_writeString("\r\n");
		
		serial_writeString("Current message size = ");
		serial_writeByte(current_message.size + '0');
		serial_writeString("\r\n");serial_writeString("\r\n");
		*/
		
		if (currentState == MESSAGE_STATE_INPROGRESS)
		{
			if (byte != MESSAGE_END_BYTE)
			{
				if (byte != MESSAGE_ESCAPE_BYTE)
				{
					current_message.data[current_message.size] = byte;
					current_message.size = current_message.size + 1;
					
				}
				else
				{
					currentState = MESSAGE_STATE_ESCAPED;
				}
			}
			else
			{
				isAvailable = 1;
			}
		}
		else if (currentState == MESSAGE_STATE_ESCAPED)
		{
			current_message.data[current_message.size] = byte;
			current_message.size = current_message.size + 1;
			currentState = MESSAGE_STATE_INPROGRESS;
		}
		else if (currentState == MESSAGE_STATE_WAIT_BEGIN && byte == MESSAGE_START_BYTE)
		{
			currentState = MESSAGE_STATE_INPROGRESS;
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
	reset();
}

void message_send(char* data, int dataSize)
{
	char buffer[dataSize*2];
	uint8_t index = 0;
	buffer[index++] = MESSAGE_START_BYTE;
	for (int i=0; i< dataSize;i++)
	{
		if (data[i] == MESSAGE_START_BYTE 
			|| data[i] == MESSAGE_END_BYTE
			|| data[i] == MESSAGE_ESCAPE_BYTE  )
		{
			buffer[index++] = MESSAGE_ESCAPE_BYTE;
		}
		buffer[index++] = data[i];
	}
	buffer[index++] = MESSAGE_END_BYTE;
	buffer[index]= 0; // Mark end of string
	serial_writeString(buffer);
}
