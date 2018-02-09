/*
 * lcd.h
 *
 * Created: 21/06/2015 16:09:46
 *  Author: JP
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

#define DDR_LCD_DATA DDRD
#define PORT_LCD_DATA PORTD
#define PIN_LCD_DATA PORTD4

#define DDR_LCD_CMD DDRD
#define PORT_LCD_CMD PORTD
#define PIN_LCD_RS PORTD2
#define PIN_LCD_ENABLE PORTD3



void init_lcd();
void write_string(char* s);
void reset_cursor();
void set_position(uint8_t row, uint8_t col);


#endif /* LCD_H_ */