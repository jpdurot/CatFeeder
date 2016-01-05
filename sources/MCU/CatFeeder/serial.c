/*
 * serial.c
 *
 * Created: 06/07/2015 22:04:57
 *  Author: JP
 */ 

#include "serial.h"

typedef struct 
{
	char data[BUFFER_SIZE];
	int pos;
	int nextData;	
} serialBuffer;

volatile serialBuffer txBuffer;
volatile serialBuffer rxBuffer;

volatile int isTransmitting = 0;

void serial_init()
{
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0); // Enable transmission and reception
	UCSR0B |= (1<<TXCIE0) | (1<<RXCIE0); // Enable interrupt on Transfer Complete
	//UCSR0B |= (1<<UDRIE0); // Enable interrupt on Transfer Complete
	UCSR0C |= (1<<UPM01)|(1<<UPM00) |(1<<UCSZ01)|(1<<UCSZ00); // Parity : Odd, Stop : 1 bit, char size : 8 bits
	// Set baud rate : UBRR = Fosc/(16*baud)-1
	UBRR0L = UBRR_VALUE;
	UBRR0H &= ~0x0F; // baud rate is defined on 12 bits, so ensure 4 lower bites are 0
	sei();
	txBuffer.pos = -1;
	txBuffer.nextData = 0;
	rxBuffer.pos = 0;
	rxBuffer.nextData = 0;
	isTransmitting = 0;
}

void serial_writeByte(char data)
{
	if (txBuffer.nextData == txBuffer.pos)
	{
		// Buffer is full, wait for some data to be sent
		while (txBuffer.nextData == txBuffer.pos);
	}
	//buffer[nextData] = ((char)nextData + 0x30);
	txBuffer.data[txBuffer.nextData]= data;
	txBuffer.nextData = (txBuffer.nextData + 1) % BUFFER_SIZE;
}

void serial_writeString(char* message)
{
	char* s = message;
	while (*s)
	{
		serial_writeByte(*s);
		s++;
	}
	if (!isTransmitting)
	{
		// Serial line is free, start sending
		isTransmitting = 1;
		txBuffer.pos = (txBuffer.pos +1) % BUFFER_SIZE;
		UDR0 = txBuffer.data[txBuffer.pos];
	}
}

// Interrupt subroutine
ISR(USART_TX_vect)
{
	volatile unsigned int newPos = (txBuffer.pos + 1) % BUFFER_SIZE;
	if (newPos == txBuffer.nextData)
	{
		UCSR0A &= ~(1<<UDRE0);
		isTransmitting = 0;
	}
	else
	{
		txBuffer.pos = newPos;
		UDR0 = txBuffer.data[txBuffer.pos];
	}
	
}

ISR(USART_RX_vect)
{
	rxBuffer.data[rxBuffer.nextData] = UDR0;
	rxBuffer.nextData = (rxBuffer.nextData + 1) % BUFFER_SIZE; 	
}

int serial_dataAvailable()
{
	return (rxBuffer.nextData - rxBuffer.pos + BUFFER_SIZE) % BUFFER_SIZE;
}

char serial_read()
{
	char result = rxBuffer.data[rxBuffer.pos];
	rxBuffer.pos = (rxBuffer.pos + 1) % BUFFER_SIZE;
	return result;
}







