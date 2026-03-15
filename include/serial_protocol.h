#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <stdint.h>

// ============================================================================
// Serial Protocol Handler
// Dual-serial architecture:
// - Hardware Serial (USB): Debug output only
// - SoftwareSerial (A1/A2): ESP32 PLC communication
//
// Protocol Format (text-based):
// Commands from ESP32:
//   "OUT:xx" - Set outputs (xx = 2 hex digits for bits 0-5)
//   "REQ"    - Request current state
//
// Responses to ESP32:
//   "STATE:in:xx" - Response with input and output states (hex)
// ============================================================================

class SerialProtocol {
public:
  /**
   * Initialize both serial ports
   */
  void init();

  /**
   * Update PLC serial communication (call regularly)
   * Processes incoming ESP32 commands
   */
  bool update();

  /**
   * Send current state to ESP32
   */
  void sendStatus();

  /**
   * Debug log to USB serial only (non-blocking)
   */
  void debugPrint(const char* msg);

private:
  static const uint8_t RX_BUFFER_SIZE = 32;
  char rx_buffer[RX_BUFFER_SIZE];
  uint8_t rx_index;

  void processCommand(const char* command);
  bool parseOutputCommand(const char* params);
};

extern SerialProtocol g_serial;

#endif
