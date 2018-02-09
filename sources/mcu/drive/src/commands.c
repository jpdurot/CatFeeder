#include "commands.h"
#include "rtc.h"

#include "serial.h"

#define I2C_CORE_ADDRESS 0x20
#define I2C_COMMAND_REGISTER 0
#define I2C_STATUS_REGISTER 1
#define I2C_CALIBRATION_REGISTER 2
#define I2C_FOOD_WEIGHT_REGISTER 7

uint8_t getVersion(char* version) {
  strcpy(version, "1.0.0");
  return 0;
}

uint8_t getCalibration(char* returnValue) {
  uint8_t registerValue;
  if (readRegister(I2C_CORE_ADDRESS, I2C_CALIBRATION_REGISTER, &registerValue ) != 0) {
    return -1;
  }
  sprintf(returnValue, "%d", registerValue);
  return 0;
}

uint8_t setCalibration(char* calibrationValue) {
  uint8_t registerValue;
  if (stringParse(calibrationValue, &registerValue) != 0) {
    return -1;
  }
  if (writeRegister(I2C_CORE_ADDRESS, I2C_CALIBRATION_REGISTER, registerValue ) != 0) {
    return -1;
  }
  *calibrationValue = 0;
  return 0;
}
