/*
* ServoDriving.c
*
* Created: 11/01/2015 21:53:44
*  Author: JP
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"
//#include "USI_TWI_Slave.h"
#include "usitwislave.h"

#define F_CPU 8000000

#include <util/delay.h>

#define I2C_SLAVE_ADDRESS	0x20

#define DDRIRR	DDRB
#define PINIRR	PINB
#define PIRR	PORTB0

#define DDRIRE	DDRA
#define PORTIRE	PORTA
#define PIRE	PORTA1

#define DDRBTN	DDRA
#define PINBTN	PINA
#define PBTN	PORTA0

#define DDRSERVO_CMD	DDRA
#define PORTSERVO_CMD	PORTA

#define SERVO1_PIN_CMD	PORTA2
#define SERVO2_PIN_CMD	PORTA3


#define SERVO1_MAX_ANGLE	160
#define SERVO1_MIN_ANGLE	2
#define SERVO1_DELAY_ANGLE	7
#define SERVO1_DELAY_MIDDLE	0

#define SERVO2_MAX_ANGLE	70
#define SERVO2_MIN_ANGLE	2
#define SERVO2_DELAY_ANGLE	20
#define SERVO2_DELAY_MIDDLE	2000

void delayMs(uint8_t ms)
{
	for (int i =0;i< ms;i++)
	{
		_delay_ms(1);
	}
}

void ir_init()
{
	DDRIRE |= _BV(PIRE); // Pin output
	DDRIRR &= ~_BV(PIRR); // Pin input
}

void setStep1()
{
	PORTSERVO_CMD &= ~_BV(SERVO2_PIN_CMD);
	PORTSERVO_CMD |= _BV(SERVO1_PIN_CMD);
}

void setStep2()
{
	PORTSERVO_CMD &= ~_BV(SERVO1_PIN_CMD);
	PORTSERVO_CMD |= _BV(SERVO2_PIN_CMD);
}

void resetServos()
{
	setStep1();
	servo_setValue(SERVO1_MAX_ANGLE);
	servo_start();
	_delay_ms(500);
	servo_stop();
	servo_setValue(SERVO2_MIN_ANGLE);
	setStep2();
	servo_start();
	_delay_ms(500);
	servo_stop();
	
}

void init()
{
	// TODO Remove debug code
	DDRA |= (1 << PORTA7);
	PORTA &= ~(1 << PORTA7);
	DDRB |= (1 << PORTB2);
	PORTB &= ~(1 << PORTB2);
	DDRB |= (1 << PORTB1);
	PORTB &= ~(1 << PORTB1);
	// End of debug code
	
	// Servo init
	servo_init();
	servo_setValue(0);
	DDRSERVO_CMD |= _BV(SERVO1_PIN_CMD) | _BV(SERVO2_PIN_CMD);
	PORTSERVO_CMD &= ~_BV(SERVO1_PIN_CMD);
	PORTSERVO_CMD &= ~_BV(SERVO2_PIN_CMD);
	
	// Init chip as I2C slave
	//USI_TWI_Slave_Initialize(I2C_SLAVE_ADDRESS);
	usi_twi_slave(I2C_SLAVE_ADDRESS);
	
	
	// IR init
	ir_init();
	
	resetServos();
}

void moveServo(uint8_t start, uint8_t end, uint16_t delayAngle)
{
	if (start > end)
	{
		for (uint8_t i=start;i>=end;i--)
		{
			servo_setValue(i);
			delayMs(delayAngle);
		}
	}
	else
	{
		for (uint8_t i=start;i<=end;i++)
		{
			servo_setValue(i);
			delayMs(delayAngle);
		}
	}
}

void process()
{
	int8_t command = getCommand();
	// Command & 0x01 ==> Load
	// Command & 0x02 ==> Feed (Load if needed)
	if (command == 0x01 || command== 0x02)
	{
		setStatus(0x01);
		PORTIRE |= _BV(PIRE); // Emit IR light
		_delay_ms(100);
		uint8_t ir = (PINIRR & _BV(PIRR));
		// Step 1
		setStep1();
		while (ir > 0)
		{
			servo_start();
			moveServo(SERVO1_MAX_ANGLE,SERVO1_MIN_ANGLE,SERVO1_DELAY_ANGLE);
			moveServo(SERVO1_MIN_ANGLE,SERVO1_MAX_ANGLE,SERVO1_DELAY_ANGLE);
			servo_stop();
			ir = (PINIRR & _BV(PIRR));
		}
		
		if (command == 0x02)
		{
			setStatus(0x02);
			// Step 2
			setStep2();
			servo_start();
			moveServo(SERVO2_MIN_ANGLE,SERVO2_MAX_ANGLE,SERVO2_DELAY_ANGLE);
			_delay_ms(SERVO2_DELAY_MIDDLE);
			moveServo(SERVO2_MAX_ANGLE,SERVO2_MIN_ANGLE,SERVO2_DELAY_ANGLE);

			servo_stop();
		}
		PORTIRE &= ~_BV(PIRE); // Stop IR light
		setCommand(0x00);
		setStatus(0x00);
	}
}

void loop()
{
	process();
	//_delay_ms(10000);
}

int main(void)
{
	init(); while(1) {loop();}
	return 0;
	
}

/*ISR(TWI_vect){

// temporary stores the received data
uint8_t data;

// own address has been acknowledged
if( (TWSR & 0xF8) == TW_SR_SLA_ACK ){
bufferAddress = I2C_NO_ADDRESS_SET;
// clear TWI interrupt flag, prepare to receive next byte and acknowledge
TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
}
else if( (TWSR & 0xF8) == TW_SR_DATA_ACK ){ // data has been received in slave receiver mode

// save the received byte inside data
data = TWDR;

// check wether an address has already been transmitted or not
if(bufferAddress == I2C_NO_ADDRESS_SET){

bufferAddress = data;

// clear TWI interrupt flag, prepare to receive next byte and acknowledge
TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
}
else{ // if a databyte has already been received


// TODO : Check register address and execute Write command


// increment the buffer address
bufferAddress++;

// if there is still enough space inside the buffer
if(bufferAddress <= I2C_MAX_REGISTER){
// clear TWI interrupt flag, prepare to receive next byte and acknowledge
TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
}
else{
// clear TWI interrupt flag, prepare to receive last byte and don't acknowledge
TWCR |= (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN);
}
}
}
else if( (TWSR & 0xF8) == TW_ST_DATA_ACK ){ // device has been addressed to be a transmitter

// copy data from TWDR to the temporary memory
data = TWDR;

// if no buffer read address has been sent yet
if( bufferAddress == I2C_NO_ADDRESS_SET ){
bufferAddress = data;
}

if (bufferAddress <= I2C_MAX_REGISTER)
{
// copy the specified buffer address into the TWDR register for transmission
TWDR = i2c_memory[bufferAddress];
}
else
{
TWDR = 0;
}
// increment buffer read address
bufferAddress++;

// if there is another buffer address that can be sent
if(bufferAddress <= I2C_MAX_REGISTER){
// clear TWI interrupt flag, prepare to send next byte and receive acknowledge
TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
}
else{
// clear TWI interrupt flag, prepare to send last byte and receive not acknowledge
TWCR |= (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN);
}

}
else{
// if none of the above apply prepare TWI to be addressed again
TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
}
}
*/





/*volatile uint8_t flags = 0x00;

#define FLAG_BUTTON 0x01

void enable_interrupts()
{
INT0_DDR &= ~(1 << INT0_BIT); // Set INT0 pin as input
//TIMSK1 = (1 << TOIE1)| (1 << OCIE1A); // Enable interrupts on Timer1 overflow and Timer1 Output Compare A
SREG |= (1 << 7); // Enable Global interrupts
GIMSK |= (1 << INT0)| (1 << PCIE1); // Enable INT0 interrupt
PCMSK1 |= (1 << PCINT10);
MCUCR |= (1 << ISC01)| (1 << ISC00);
sei(); // Activate interrupts


}*/