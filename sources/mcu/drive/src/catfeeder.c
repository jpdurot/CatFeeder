#include "messaging.h"
#include "at.h"
#include <string.h>
#include <stdlib.h>

// Debug
#include "serial.h"


void init() {
  serial_init();
  serial_writeString("Serial bus is ready !\r\n");
  serial_writeString("Init I2C ... ");
  rtc_init(); // For I2C, before splitting I2C part from RTC one
  serial_writeString("OK\r\n");
  serial_writeString("Init messaging system... ");
  message_init();
  serial_writeString("OK\r\n");
}

int main() {
  init();
  serial_writeString("Init OK, ready to listen to commands !\r\n");
  while (1) {
    if (message_available()) {
      message m;
      message_get(&m);
      serial_writeString("Message received : ");
      serial_writeString(m.data);
      serial_writeString("\r\n");
      at_command command;
      char returnString[40];
      uint8_t ret = executeATCommand(&m, returnString);
      if (ret) {
        serial_writeString("ERR\r\n");
      } else {
        if (strlen(returnString) == 0) {
          serial_writeString("OK!\r\n");
        } else {
          serial_writeString(returnString);
          serial_writeString("\r\n");
        }
      }

    }
  }
}
