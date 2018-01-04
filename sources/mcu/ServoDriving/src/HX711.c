#include "HX711.h"
#include "utils.h"


void HX711_init()
{

  // Set CLK as output
  SETOUTPUT(PIN_CLK);
  // Set DOUT as input
  SETINPUT(PIN_DOUT);
}

void waitForReady()
{
  CLEARBIT(PIN_CLK);
  while (READBIT(PIN_DOUT));
}

uint32_t HX711_read()
{
  waitForReady();
  uint32_t value = 0;
  for (int i = 0; i< 24;i++)
  {

    // positive pulse on CLK
    SETBIT(PIN_CLK);
    value = value<<1;
    // Read DOUT
    //Serial.print("DOUT = ");
    //Serial.println(READBIT(PIN_DOUT));
    CLEARBIT(PIN_CLK);
    if (READBIT(PIN_DOUT))
      value++;


  }
  // 25th positive pulse (gain 128)
  SETBIT(PIN_CLK);
  CLEARBIT(PIN_CLK);
  return value^0x800000;
}
