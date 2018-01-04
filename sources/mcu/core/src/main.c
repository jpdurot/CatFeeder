#define F_CPU 8000000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c_slave.h"
#include "weightSensor.h"
#include "utils.h"

#include "feeder.h"



#define A1 A,1

void blinkReady()
{
  for (int i=0;i<3;i++)
  {
    SETBIT(A1);
    _delay_ms(100);
    CLEARBIT(A1);
    _delay_ms(50);
  }
}


int main()
{
  _delay_ms(1000);
  feeder_init();
  usi_twi_slave(0x20);
  setCommand(0);
  SETOUTPUT(A1);
  blinkReady();
  while (1)
  {

    if (getCommand() == 1)
    {
      SETBIT(A1);

      // Get calibration from I2C registers
      //uint32_t calibration = ((getRegister(2) & 0xff) << 24) + ((getRegister(3) & 0xff) << 16) + ((getRegister(4) & 0xff)) << 8 + (getRegister(5));
      uint32_t calibration = i2c_getRegister(2);
      weight_setCalibration(calibration);
      uint32_t weight = weight_getWeight();
      //cli();
      i2c_setRegister(3,(weight & 0xff000000)>> 24);
      i2c_setRegister(4,(weight & 0xff0000)>> 16);
      i2c_setRegister(5,(weight & 0xff00)>> 8);
      i2c_setRegister(6,(weight & 0xff));
      setCommand(0);
      //sei();
    }
    else if (getCommand() == 2)
    {
      SETBIT(A1);
      _delay_ms(100);
      CLEARBIT(A1);
      _delay_ms(100);
      SETBIT(A1);
      weight_setTare();
      //cli();
      setCommand(0);
      //sei();
    }

    else if (getCommand() == 3)
    {
      feeder_feed();
      setCommand(0);
    }

    /*else if (getCommand() == 4)
    {
      SETOUTPUT()
    }*/

    CLEARBIT(A1);
  }
}
