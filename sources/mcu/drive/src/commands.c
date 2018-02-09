#include "commands.h"
#include "rtc.h"

uint8_t getVersion(char* version) {
  strcpy(version, "1.0.0");
  return 0;
}
