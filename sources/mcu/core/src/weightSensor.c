#include "weightSensor.h"

#define NB_READ 3

uint32_t tareValue;
uint8_t calibration;

uint32_t _readMultiple()
{
  uint32_t total =0;
  for (int i=0;i<NB_READ;i++)
  {
    total += HX711_read();
  }
  return total / NB_READ;
}

void weight_init()
{
  HX711_init();
  weight_setTare();
}

void weight_setTare()
{
  tareValue = _readMultiple();
}

void weight_setCalibration(uint8_t value)
{
  calibration = value;
}


int32_t weight_getWeight()
{
  int32_t value = _readMultiple();
  uint8_t isNegative = 0;
  value -= tareValue;
  if (value < 0)
  {
    isNegative = 1;
    value *= -1;
  }
  value = value / calibration;
  if (isNegative)
  {
    value *= -1;
  }
  return value;
}
