/*
 * messaging.h
 *
 * Created: 16/07/2015 22:01:56
 *  Author: JP
 */ 


#ifndef MESSAGING_H_
#define MESSAGING_H_

#define MESSAGE_MAX_SIZE 20

#define MESSAGE_START_BYTE 0x41
#define MESSAGE_END_BYTE 0x5A
#define MESSAGE_ESCAPE_BYTE 0xFD
#define MESSAGE_ESCAPED_XOR 0x40

#define MESSAGE_STATE_WAIT_BEGIN 1
#define MESSAGE_STATE_INPROGRESS 2
#define MESSAGE_STATE_ESCAPED 3


typedef struct  
{
	int size;
	char data[MESSAGE_MAX_SIZE];
}message;

void message_init();

int message_available();

void message_get(message* msg);

void message_send(char* data, int dataSize);



#endif /* MESSAGING_H_ */