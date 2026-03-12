#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <stdint.h>

class SerialProtocol {
public:
  void init();
  bool update();
  void sendStatus();
private:
  static const uint8_t RX_BUFFER_SIZE = 32;
  char rx_buffer[RX_BUFFER_SIZE];
  uint8_t rx_index;

  void processCommand(const char* command);
  bool parseOutputCommand(const char* params);
};

extern SerialProtocol g_serial;

#endif
