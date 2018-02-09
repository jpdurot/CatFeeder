#include "messaging.h"
#include "at.h"
#include <string.h>
#include <stdlib.h>

// Debug
#include "serial.h"








int main() {
  message_init();
  serial_writeString("Init OK\r\n");
  while (1) {
    if (message_available()) {
      message m;
      message_get(&m);
      serial_writeString("Message received : ");
      serial_writeString(m.data);
      serial_writeString("\r\n");
      at_command command;
      char* returnString = malloc(sizeof(char) * 40);
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
