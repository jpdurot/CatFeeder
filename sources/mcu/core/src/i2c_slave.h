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

#define I2C_MAX_REGISTER	0x07
#define I2C_CMD_REGISTER	0x00
#define I2C_STATUS_REGISTER	0x01
#define I2C_NO_ADDRESS_SET	0xFF

/* I2C register definition
* 0 : Command
* 1 : Status
* 2 : Weigh sensor Calibration
* 3-6 : last computed weight (R3<<24 + R4<<8 + R5<<8 + R6)
* 7 : Food weight in grams
*/

void		i2c_set_slave_id(uint8_t slave_address);


uint8_t getStatus();
void setStatus(uint8_t value);
uint8_t getCommand();
void setCommand(uint8_t value);
uint8_t i2c_getRegister(uint8_t index);
void i2c_setRegister(uint8_t index, uint8_t value);


#endif /* USITWISLAVE_H_ */
