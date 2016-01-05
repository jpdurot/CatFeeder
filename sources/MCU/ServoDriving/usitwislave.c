/*	See LICENSE for Copyright etc. */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "usitwislave.h"

enum
{
	of_state_check_address,
	of_state_send_data,
	of_state_request_ack,
	of_state_check_ack,
	of_state_receive_data,
	of_state_store_data_and_send_ack
} overflow_state_t;

enum
{
	ss_state_before_start,
	ss_state_after_start,
	ss_state_address_selected,
	ss_state_address_not_selected,
	ss_state_data_processed
} startstop_state_t;

static uint8_t of_state;
static uint8_t ss_state;

static uint8_t	slave_address;

static uint8_t	stats_enabled;
static uint16_t	start_conditions_count;
static uint16_t	stop_conditions_count;
static uint16_t	error_conditions_count;
static uint16_t	overflow_conditions_count;
static uint16_t	local_frames_count;
static uint16_t	idle_call_count;

volatile uint8_t i2c_memory[I2C_MAX_REGISTER + 1];

//volatile uint8_t statusRegister =0x00;
//volatile uint8_t commandRegister = 0x00;

volatile uint8_t bufferAddress = I2C_NO_ADDRESS_SET;

uint8_t getStatus()
{
	//return statusRegister;
	return i2c_memory[I2C_STATUS_REGISTER];
}

void setStatus(uint8_t value)
{
	//statusRegister = value;
	i2c_memory[I2C_STATUS_REGISTER] = value;
}

uint8_t getCommand()
{
	//return commandRegister;
	return i2c_memory[I2C_CMD_REGISTER];
}

void setCommand(uint8_t value)
{
	//commandRegister = value;
	i2c_memory[I2C_CMD_REGISTER] = value;
}

static void set_sda_to_input(void)
{
	DDR_USI &= ~_BV(PORT_USI_SDA);
}

static void set_sda_to_output(void)
{
	DDR_USI |= _BV(PORT_USI_SDA);
}

static inline void set_scl_to_input(void)
{
	DDR_USI &= ~_BV(PORT_USI_SCL);
}

static inline void set_scl_to_output(void)
{
	DDR_USI |= _BV(PORT_USI_SCL);
}

static inline void set_sda_low(void)
{
	PORT_USI &= ~_BV(PORT_USI_SDA);
}

static inline void set_sda_high(void)
{
	PORT_USI |= _BV(PORT_USI_SDA);
}

static inline void set_scl_low(void)
{
	PORT_USI &= ~_BV(PORT_USI_SCL);
}

static inline void set_scl_high(void)
{
	PORT_USI |= _BV(PORT_USI_SCL);
}

static inline void twi_reset_state(void)
{
	USISR =
	(1		<< USISIF)	|		// clear start condition flag
	(1		<< USIOIF)	|		// clear overflow condition flag
	(0		<< USIPF)	|		// !clear stop condition flag
	(1		<< USIDC)	|		// clear arbitration error flag
	(0x00	<< USICNT0);		// set counter to "8" bits

	USICR =
	(1 << USISIE) |									// enable start condition interrupt
	(0 << USIOIE) |									// !enable overflow interrupt
	(1 << USIWM1) | (0 << USIWM0) |					// set usi in two-wire mode, disable bit counter overflow hold
	(1 << USICS1) | (0 << USICS0) | (0 << USICLK) |	// shift register clock source = external, positive edge, 4-bit counter source = external, both edges
	(0 << USITC);									// don't toggle clock-port pin
	bufferAddress = I2C_NO_ADDRESS_SET; 
}

static void twi_reset(void)
{
	// make sure no sda/scl remains pulled up or down

	set_sda_to_input();		//	deactivate internal pullup on sda/scl
	set_sda_low();
	set_scl_to_input();
	set_scl_low();

	set_sda_to_output();	//	release (set high) on sda/scl
	set_sda_high();
	set_sda_to_input();
	set_sda_high();
	set_scl_to_output();
	set_scl_high();

	twi_reset_state();
}

static inline void twi_init(void)
{
	#if defined(USIPP)
	#if  defined(USI_ON_PORT_A)
	USIPP |= _BV(USIPOS);
	#else
	USIPP &= ~_BV(USIPOS);
	# endif
	#endif

	twi_reset();
}

ISR(USI_START_VECTOR)
{
	set_sda_to_input();

	// wait for SCL to go low to ensure the start condition has completed (the
	// start detector will hold SCL low) - if a stop condition arises then leave
	// the interrupt to prevent waiting forever - don't use USISR to test for stop
	// condition as in Application Note AVR312 because the stop condition Flag is
	// going to be set from the last TWI sequence

	while(!(PIN_USI & _BV(PIN_USI_SDA)) &&
	(PIN_USI & _BV(PIN_USI_SCL)))

	// possible combinations
	//	sda = low	scl = low		break	start condition
	// 	sda = low	scl = high		loop
	//	sda = high	scl = low		break	stop condition
	//	sda = high	scl = high		break	stop condition

	if((PIN_USI & _BV(PIN_USI_SDA)))	// stop condition
	{
		twi_reset();

		if(stats_enabled)
		error_conditions_count++;
		return;
	}

	if(stats_enabled)
	start_conditions_count++;

	of_state = of_state_check_address;
	ss_state = ss_state_after_start;
	//bufferAddress = I2C_NO_ADDRESS_SET;

	USIDR = 0xff;

	USICR =
	(1 << USISIE) |									// enable start condition interrupt
	(1 << USIOIE) |									// enable overflow interrupt
	(1 << USIWM1) | (1 << USIWM0) |					// set usi in two-wire mode, enable bit counter overflow hold
	(1 << USICS1) | (0 << USICS0) | (0 << USICLK) |	// shift register clock source = external, positive edge, 4-bit counter source = external, both edges
	(0 << USITC);									// don't toggle clock-port pin

	USISR =
	(1		<< USISIF)	|		// clear start condition flag
	(1		<< USIOIF)	|		// clear overflow condition flag
	(0		<< USIPF)	|		// !clear stop condition flag
	(1		<< USIDC)	|		// clear arbitration error flag
	(0x00	<< USICNT0);		// set counter to "8" bits
}

ISR(USI_OVERFLOW_VECTOR)
{
	// bit shift register overflow condition occured
	// scl forced low until overflow condition is cleared!

	uint8_t data		= USIDR;
	uint8_t set_counter = 0x00;		// send 8 bits (16 edges)

	if(stats_enabled)
	overflow_conditions_count++;

	again:
	switch(of_state)
	{
		// start condition occured and succeed
		// check address, if not OK, reset usi
		// note: not using general call address

		case(of_state_check_address):
		{
			uint8_t address;
			uint8_t direction;

			direction	= data & 0x01;
			address		= (data & 0xfe) >> 1;

			if(address == slave_address)
			{
				ss_state = ss_state_address_selected;

				if(direction)					// read request from master
				of_state = of_state_send_data;
				else							// write request from master
				of_state = of_state_receive_data;

				USIDR		= 0x00;
				set_counter = 0x0e;				// send 1 bit (2 edges)
				set_sda_to_output();			// initiate send ack
			}
			else
			{
				USIDR		= 0x00;
				set_counter = 0x00;
				twi_reset_state();
				ss_state = ss_state_address_not_selected;
			}

			break;
		}

		// process read request from master

		case(of_state_send_data):
		{
			ss_state = ss_state_data_processed;
			of_state = of_state_request_ack;
			
			if ( bufferAddress <= I2C_MAX_REGISTER )
			{
				//if (bufferAddress == 0x00)
				//	USIDR = commandRegister;
				//if (bufferAddress == 0x01)
				//	USIDR = statusRegister;	
				USIDR = i2c_memory[bufferAddress];
				bufferAddress++;
			}
			else
			{
				USIDR = 0x00;					// no more data, but cannot send "nothing" or "nak"
			}			

			set_counter = 0x00;
			set_sda_to_output();				// initiate send data

			break;
		}

		// data sent to master, request ack (or nack) from master

		case(of_state_request_ack):
		{
			of_state = of_state_check_ack;

			USIDR		= 0x00;
			set_counter = 0x0e;					//	receive 1 bit (2 edges)
			set_sda_to_input();					//	initiate receive ack

			break;
		}

		// ack/nack from master received

		case(of_state_check_ack):
		{
			if(data)	// if NACK, the master does not want more data
			{
				of_state = of_state_check_address;
				set_counter = 0x00;
				twi_reset();
			}
			else
			{
				of_state = of_state_send_data;
				goto again;	// from here we just drop straight into state_send_data
			}				// don't wait for another overflow interrupt

			break;
		}

		// process write request from master

		case(of_state_receive_data):
		{
			ss_state = ss_state_data_processed;

			of_state = of_state_store_data_and_send_ack;

			set_counter = 0x00;					// receive 1 bit (2 edges)
			set_sda_to_input();					// initiate receive data

			break;
		}

		// data received from master, store it and wait for more data

		case(of_state_store_data_and_send_ack):
		{
			of_state = of_state_receive_data;

			if (bufferAddress == I2C_NO_ADDRESS_SET)
			{
				
				bufferAddress = data;
			}
			else if (bufferAddress <= I2C_MAX_REGISTER)
			{
				i2c_memory[bufferAddress] = data;
				bufferAddress++;
			}
			else
			{
				 //Nothing to do
			}

			USIDR		= 0x00;
			set_counter = 0x0e;					// send 1 bit (2 edges)
			set_sda_to_output();				// initiate send ack

			break;
		}
	}

	USISR =
	(0				<< USISIF)	|		// don't clear start condition flag
	(1				<< USIOIF)	|		// clear overflow condition flag
	(0				<< USIPF)	|		// don't clear stop condition flag
	(1				<< USIDC)	|		// clear arbitration error flag
	(set_counter	<< USICNT0);		// set counter to 8 or 1 bits
}

void usi_twi_slave(uint8_t slave_address_in)
{
	slave_address			= slave_address_in;

	ss_state				= ss_state_before_start;

	twi_init();
	setStatus(0x00);
	sei();
}

void usi_twi_enable_stats(uint8_t onoff)
{
	stats_enabled				= onoff;
	start_conditions_count		= 0;
	stop_conditions_count		= 0;
	error_conditions_count		= 0;
	overflow_conditions_count	= 0;
	local_frames_count			= 0;
	idle_call_count				= 0;
}

uint16_t usi_twi_stats_start_conditions(void)
{
	return(start_conditions_count);
}

uint16_t usi_twi_stats_stop_conditions(void)
{
	return(stop_conditions_count);
}

uint16_t usi_twi_stats_error_conditions(void)
{
	return(error_conditions_count);
}

uint16_t usi_twi_stats_overflow_conditions(void)
{
	return(overflow_conditions_count);
}

uint16_t usi_twi_stats_local_frames(void)
{
	return(local_frames_count);
}

uint16_t usi_twi_stats_idle_calls(void)
{
	return(idle_call_count);
}