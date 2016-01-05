/*
 * servo.h
 *
 * Created: 15/01/2015 19:29:30
 *  Author: JP
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <avr/io.h>

void servo_init();
void servo_start();
void servo_setValue(uint8_t angle);
void servo_stop();





#endif /* SERVO_H_ */