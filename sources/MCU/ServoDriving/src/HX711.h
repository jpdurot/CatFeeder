#include <avr/io.h>
#include <stdint.h>

#define PIN_CLK B,1
#define PIN_DOUT B,2

void HX711_init();
uint32_t HX711_read();
