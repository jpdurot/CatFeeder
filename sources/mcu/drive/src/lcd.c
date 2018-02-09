/*
 * lcd.c
 *
 * Created: 21/06/2015 16:10:09
 *  Author: JP
 */ 

#include "lcd.h"

void long_pulse_enable()
{
	// Set ENABLE high
	PORT_LCD_CMD |= (1 << PIN_LCD_ENABLE);
	_delay_ms(1);
	// Set ENABLE low
	PORT_LCD_CMD &= ~(1 << PIN_LCD_ENABLE);
	_delay_ms(1);
}

void pulse_enable()
{
	// Set ENABLE high
	PORT_LCD_CMD |= (1 << PIN_LCD_ENABLE);
	_delay_us(1000);
	// Set ENABLE low
	PORT_LCD_CMD &= ~(1 << PIN_LCD_ENABLE);
	_delay_us(1000); 
}

void send_command(uint8_t value)
{
	// Set RS low
	PORT_LCD_CMD &= ~(1 << PIN_LCD_RS);
	uint8_t high = value >> 4;
	uint8_t low = value & 0x0F;
	
	PORT_LCD_DATA &= ~ (0b1111 << PIN_LCD_DATA);
	PORT_LCD_DATA |= high << PIN_LCD_DATA;
	
	pulse_enable();
	
	PORT_LCD_DATA &= ~ (0b1111 << PIN_LCD_DATA);
	PORT_LCD_DATA |= low << PIN_LCD_DATA;

	pulse_enable();
}

void function_set()
{
	// Set 4 bits mode, without parameters
	PORT_LCD_DATA |= (0b0011 << PIN_LCD_DATA);
	long_pulse_enable();
	_delay_ms(5);
	long_pulse_enable();
	long_pulse_enable();
	PORT_LCD_DATA &= ~(1 << PIN_LCD_DATA) ; //Set DB4 low
	long_pulse_enable();
	send_command(0b00101000);
	send_command(0b00001000);
	send_command(0b00000001);
}

void display_control_set()
{
	send_command(0b00001100); // D=1 ==> Display is on : C =0 ==> No cursor : B=0 ==> No blinking
}

void entry_mode_set()
{
	send_command(0b00000110);  // I/D= 1 ==> Increment : S = 0 ==> : No shifting
}

void init_lcd()
{
	// Set data, RS and ENABLE pin as output
	DDR_LCD_DATA |= 0b00001111 << PIN_LCD_DATA;
	DDR_LCD_CMD |= 1 << PIN_LCD_RS;
	DDR_LCD_CMD |= 1 << PIN_LCD_ENABLE;
	
	// Set RS and ENABLE as low
	PORT_LCD_CMD &= ~(1 << PIN_LCD_RS);
	PORT_LCD_CMD &= ~(1 << PIN_LCD_ENABLE);

	_delay_ms(100);
	function_set();
	display_control_set();
	entry_mode_set();
}

void clear_screen()
{
	send_command(0b00000001);
}

void reset_cursor()
{
	send_command(0b00000010);
}

void set_position(uint8_t row, uint8_t col)
{
	uint8_t address = col + 40 * row;
	address = address  | 0b10000000;
	send_command(address);
}
void write_char(char c)
{
	// Set RS high
	PORT_LCD_CMD |= (1 << PIN_LCD_RS);
	uint8_t high = c >> 4;
	uint8_t low = c & 0x0F;
	
	PORT_LCD_DATA &= ~ (0b1111 << PIN_LCD_DATA);
	PORT_LCD_DATA |= high << PIN_LCD_DATA;
	
	pulse_enable();
	
	PORT_LCD_DATA &= ~ (0b1111 << PIN_LCD_DATA);
	PORT_LCD_DATA |= low << PIN_LCD_DATA;

	pulse_enable();
}

void write_string(char* s)
{
	while (*s)
	{
		write_char(*s);
		s++;
	}
}



