#include <stdint.h>
#include "HX711.h"

void weight_init();
void weight_setCalibration(uint8_t value);
void weight_setTare();
int32_t weight_getWeight();
