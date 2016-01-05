/*
 * usitwislave.h
 *
 * Created: 01/11/2015 23:04:39
 *  Author: JP
 */ 


#ifndef USITWISLAVE_H_
#define USITWISLAVE_H_


/*	See LICENSE for Copyright etc. */

#include <stdint.h>

//! ATTiny84 defines
#define DDR_USI             DDRA
#define PORT_USI            PORTA
#define PIN_USI             PINA
#define PORT_USI_SDA        PORTA6
#define PORT_USI_SCL        PORTA4
#define PIN_USI_SDA         PINA6
#define PIN_USI_SCL         PINA4
#define USI_START_COND_INT  USISIF
#define USI_START_VECTOR    USI_STR_vect
#define USI_OVERFLOW_VECTOR USI_OVF_vect

#define I2C_MAX_REGISTER	0x01
#define I2C_CMD_REGISTER	0x00
#define I2C_STATUS_REGISTER	0x01
#define I2C_NO_ADDRESS_SET	0xFF

void		usi_twi_slave(uint8_t slave_address);

void		usi_twi_enable_stats(uint8_t onoff);
uint16_t	usi_twi_stats_start_conditions(void);
uint16_t	usi_twi_stats_stop_conditions(void);
uint16_t	usi_twi_stats_error_conditions(void);
uint16_t	usi_twi_stats_overflow_conditions(void);
uint16_t	usi_twi_stats_local_frames(void);
uint16_t	usi_twi_stats_idle_calls(void);

uint8_t getStatus();
void setStatus(uint8_t value);
uint8_t getCommand();
void setCommand(uint8_t value);


#endif /* USITWISLAVE_H_ */