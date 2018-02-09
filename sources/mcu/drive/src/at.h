#ifndef AT_H_
#define AT_H_

#include "messaging.h"
#include <stdint.h>

#define AT_KEY_LENGTH 10
#define AT_VALUE_LENGTH 20

typedef struct {
  char key[AT_KEY_LENGTH];
  char value[AT_VALUE_LENGTH];
  uint8_t hasValue;
}at_command;


uint8_t executeATCommand(const message* message, char* returnString);

#endif
