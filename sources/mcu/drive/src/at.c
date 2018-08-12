#include "at.h"
#include <string.h>

#include "commands.h"

typedef uint8_t (*at_command_get)(char* returnString);
typedef uint8_t (*at_command_set)(char* value);



typedef struct  {
  char name[AT_KEY_LENGTH];
  at_command_get get;
  at_command_set set;

} at_command_definition;

#define AT_NB_DEFINITIONS 3
at_command_definition at_definitions[AT_NB_DEFINITIONS] = {
   {.name="VER", .get=getVersion, .set=0},
   {.name="CALIB", .get=getCalibration, .set=setCalibration},
   {.name="FEED", .get=feed, .set=0}
};




uint8_t parseATCommand(const message* message, at_command* command) {
  if (message->size < 4 || message->size > (3 + AT_KEY_LENGTH +  1 + AT_VALUE_LENGTH))
    return -1;
  if (message->data[0] != 'A' || message->data[1] != 'T' || message->data[2] != '+' || message->data[3] == '=')
    return -1;
  uint8_t index = 3;
  command->hasValue = 0;
  uint8_t destIndex = 0;
  while (index < message->size) {
    if (message->data[index] != '=' || command->hasValue) {
      if (command->hasValue) {
        command->value[destIndex++] = message->data[index];
      } else {
        command->key[destIndex++] = message->data[index];
      }
    } else {
      command->key[destIndex] = 0;
      destIndex = 0;
      command->hasValue = 1;
    }
    index++;
  }
  if (destIndex == 0)
    return -1;
  if (command->hasValue) {
    command->value[destIndex] = 0;
  } else {
    command->key[destIndex] = 0;
  }
  return 0;
}

uint8_t executeATCommand(const message* message, char* returnString) {
  at_command command;
  if (parseATCommand(message, &command) != 0) {
    return -1;
  }
  // Find if command is supported
  uint8_t i = 0;
  while (i< AT_NB_DEFINITIONS) {
    if (strcmp(command.key, at_definitions[i].name) == 0) {
      if (command.hasValue) {
        if (at_definitions[i].set != 0) {
          returnString[0] = 0; // Set to empty string
          return at_definitions[i].set(command.value);
        } else
        return -1;
      } else {
        if (at_definitions[i].get != 0) {
          return at_definitions[i].get(returnString);
        } else
        return -1;
      }
      return -1;
    }
    i++;
  }
  return -1;
}
