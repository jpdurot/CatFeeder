/*
 * servo.c
 *
 * Created: 15/01/2015 19:31:44
 *  Author: JP
 */ 

#include "servo.h"
#include <avr/io.h>

#define	TICKS_PER_CYCLE		20000
#define	MIN_TICKS			544
#define	MAX_TICKS			2400

void servo_init()
{
	DDRA |= (1 << PA5);
	TCCR1A = (1 << COM1B1)|(1 << WGM11);	// Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)
	// 2 low bits for mode 14 : Fast PWM, TOP on ICR1
	
	TCCR1B = (1 << WGM13)| (1 << WGM12);	// 2 high bits for mode 14 : Fast PWM, TOP on ICR1
	
	ICR1 = TICKS_PER_CYCLE;
}
void servo_start()
{
	// Prescaler 8
	TCCR1B |= (1 << CS11);
	// Reset counter value
	TCNT1 = 0;
}

void servo_stop()
{
	// Remove prescaler 8 ==> CS13-12-11 = 0 ==> Disable counter
	TCCR1B &= ~(1 << CS11);
}

void servo_setValue(uint8_t angle)
{
	uint16_t microSeconds = MIN_TICKS + (MAX_TICKS-MIN_TICKS)*(uint32_t)angle/180;
	OCR1B = microSeconds;
}
