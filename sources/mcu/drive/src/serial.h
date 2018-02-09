/*
 * serial.h
 *
 * Created: 06/07/2015 22:04:45
 *  Author: JP
 */


#ifndef SERIAL_H_
#define SERIAL_H_

#include "global.h"
#include "avr/interrupt.h"
#include "avr/io.h"

#define BAUDRATE     9600
#define UBRR_VALUE   (F_CPU/16/BAUDRATE-1)
#define BUFFER_SIZE  100

void serial_init();

void serial_writeChar(char data);
void serial_writeString(char* message);
void serial_writeInt(uint8_t value);

int serial_dataAvailable();

char serial_read();



#endif /* SERIAL_H_ */
