#include "feeder.h"

//#define FEEDER_TEST 1

#include <util/delay.h>

#include "utils.h"
#include "servo.h"
#include "i2c_slave.h"
#include "weightSensor.h"

uint16_t desiredWeight = 400;

#define SERVO1_MAX_ANGLE	   160
#define SERVO1_MIN_ANGLE	   2
#define SERVO1_DELAY_ANGLE	 7
#define SERVO1_DELAY_MIDDLE  0
#define SERVO1_ANGLE         (SERVO1_MAX_ANGLE - SERVO1_MIN_ANGLE)

#define SERVO2_MAX_ANGLE	   140
#define SERVO2_MIN_ANGLE	   2
#define SERVO2_DELAY_ANGLE	 15
#define SERVO2_DELAY_MIDDLE	 2000


#define SERVO1_PIN_CMD	A,2
#define SERVO2_PIN_CMD	A,3

#define I2C_REG_CALIBRATION 2
#define I2C_REG_WEIGHT 3
#define I2C_REG_FOOD_WEIGHT 7

//#define A1 A,1


// Private functions

void setStep1()
{
  CLEARBIT(SERVO2_PIN_CMD);
  SETBIT(SERVO1_PIN_CMD);
}

void setStep2()
{
  CLEARBIT(SERVO1_PIN_CMD);
  SETBIT(SERVO2_PIN_CMD);
}

// Returns weight and set I2C register with the new value
int32_t updateWeight()
{
  int32_t weight = weight_getWeight();
  i2c_setRegister(I2C_REG_WEIGHT,(weight & 0xff000000)>> 24);
  i2c_setRegister(I2C_REG_WEIGHT + 1,(weight & 0xff0000)>> 16);
  i2c_setRegister(I2C_REG_WEIGHT + 2,(weight & 0xff00)>> 8);
  i2c_setRegister(I2C_REG_WEIGHT + 3,(weight & 0xff));
  return weight;
}

void delayMs(uint16_t ms)
{
	for (uint16_t i =0;i< ms;i++)
	{
		_delay_ms(1);
	}
}


void feeder_init()
{
  servo_init();
  weight_init();
  SETOUTPUT(SERVO1_PIN_CMD);
  SETOUTPUT(SERVO2_PIN_CMD);
}

void servo_resetPosition()
{
  setStep1();
  servo_setValue(SERVO1_MAX_ANGLE);
  servo_start();
  delayMs(1500);
  servo_stop();
  setStep2();
  servo_setValue(SERVO2_MIN_ANGLE);
  servo_start();
  delayMs(1500);
  servo_stop();

}

void feeder_feed()
{
  servo_resetPosition();

  // Assume feed is empty
#ifndef FEEDER_TEST
  weight_setTare();
  uint8_t calibration = i2c_getRegister(I2C_REG_CALIBRATION);
  desiredWeight = i2c_getRegister(I2C_REG_FOOD_WEIGHT) * 10;
  weight_setCalibration(calibration);
  int32_t weight = updateWeight();
#else
  int32_t weight = 0;
#endif
  setStep1();
  servo_start();
#ifdef FEEDER_TEST
  uint8_t index;
  while (index++ < 1)
#else
  while (weight < desiredWeight)
#endif
  {
    // Define when to check weight during servo move
    // If we are close to the desired weight, check more often
    uint8_t angleModulo = SERVO1_ANGLE;
    if (weight < (desiredWeight - 200))
    {
      // No check
      angleModulo = SERVO1_ANGLE;
    }
    else if (weight < (desiredWeight - 100))
    {
      // 2 checks
      angleModulo = SERVO1_ANGLE / 4;
    }
    else
    {
      // 3 checks
      angleModulo = SERVO1_ANGLE / 6;
    }

    // Move servo to push cat food
    uint8_t index = 0;
    for (int i = SERVO1_MIN_ANGLE;i <= SERVO1_MAX_ANGLE;i++)
    {

      servo_setValue(i);
			delayMs(SERVO1_DELAY_ANGLE);
    }

    // Move back to the initial position
    delayMs(SERVO1_DELAY_MIDDLE);
    for (int i = SERVO1_MAX_ANGLE;i >= SERVO1_MIN_ANGLE;i--)
    {
      if (index % angleModulo == 0)
      {
        //Check weight
        #ifndef FEEDER_TEST
        weight = updateWeight();
        if (weight >= desiredWeight)
          break;
        #endif
      }
      servo_setValue(i);
			delayMs(SERVO1_DELAY_ANGLE);
      index++;
    }
      #ifndef FEEDER_TEST
    // Check weight a last time
    weight = updateWeight();
    #endif
  }
  // Here we expect feeder has the expected weight
  // Move servo 2 so that food fall on the bowl
  setStep2();
  for (int i = SERVO2_MIN_ANGLE;i <= 70;i++)
  {
    servo_setValue(i);
    delayMs(SERVO2_DELAY_ANGLE);
  }
  for (int i = 71;i <= SERVO2_MAX_ANGLE;i++)
  {
    servo_setValue(i);
    delayMs(SERVO2_DELAY_ANGLE * 3);
  }
  delayMs(SERVO2_DELAY_MIDDLE);
  for (int i = SERVO2_MAX_ANGLE;i >= SERVO2_MIN_ANGLE;i--)
  {
    servo_setValue(i);
    delayMs(SERVO2_DELAY_ANGLE);
  }
  servo_stop();
}
