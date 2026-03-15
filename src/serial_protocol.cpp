#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "serial_protocol.h"
#include "config.h"
#include "inputs.h"
#include "outputs.h"

SerialProtocol g_serial;

/* SoftwareSerial for ESP32 communication (A1=RX, A2=TX) */
SoftwareSerial plc_link(PLC_SERIAL_RX_PIN, PLC_SERIAL_TX_PIN);

/*
* @brief Initialize serial communication and internal state
*/
void SerialProtocol::init() {
  /* Initialize debug serial (USB) */
  Serial.begin(DEBUG_SERIAL_BAUD);
  
  /* Initialize PLC serial (ESP32 link via SoftwareSerial) */
  plc_link.begin(PLC_SERIAL_BAUD);
  
  rx_index = 0;
  memset(rx_buffer, 0, RX_BUFFER_SIZE);
}

/**
 * @brief Update PLC serial communication and process incoming commands
 * @return true if a command was processed, false otherwise
 */
bool SerialProtocol::update() {
  bool command_processed = false;

  while (plc_link.available() > 0) {
    char c = plc_link.read();

    /* Handle different terminators */ 
    if (c == '\n' || c == '\r') {
      if (rx_index > 0) {
        rx_buffer[rx_index] = '\0';
        
        /* Strip any trailing whitespace/control characters */
        while (rx_index > 0 && (rx_buffer[rx_index - 1] == ' ' || rx_buffer[rx_index - 1] == '\t')) {
          rx_buffer[--rx_index] = '\0';
        }
        
        processCommand(rx_buffer);
        command_processed = true;
        rx_index = 0;
      }
    } else if (rx_index < RX_BUFFER_SIZE - 1) {
      rx_buffer[rx_index++] = c;
    }
  }

  return command_processed;
}

/**
 * @brief Process a received command from ESP32
 * Expected commands:
 * - "OUT:xx" to set outputs (xx = hex value)
 * - "REQ" to request current state
 * @param command The command string to process
 */
void SerialProtocol::processCommand(const char* command) {

  if (strncmp(command, "OUT:", 4) == 0) {
    /* Output control command */
    parseOutputCommand(&command[4]);
  } else if (strcmp(command, "REQ") == 0) {
    /* Status request */
    sendStatus();
  } else {
    /* Unknown command - debug log */
    debugPrint("ERR:Unknown PLC command");
  }
}

/**
 * @brief Parse an output control command
 * @param params The parameter string following the command
 * @return true if the command was parsed successfully, false otherwise
 */
bool SerialProtocol::parseOutputCommand(const char* params) {
  /* Parse hex string (max 2 characters for 6 outputs) */
  if (strlen(params) < 2) {
    debugPrint("ERR:Invalid output command");
    return false;
  }

  /* Convert hex string to byte value */
  unsigned int temp_value = 0;
  if (sscanf(params, "%02x", &temp_value) == 1) {
    /* Apply only the 6 valid output bits (mask 0x3F) */
    uint8_t output_value = (uint8_t)(temp_value & 0x3F);
    g_outputs.setAllStates(output_value);
    /* Send status response after command is processed */
    sendStatus();
    return true;
  } else {
    debugPrint("ERR:Invalid hex value");
    return false;
  }
}

/**
 * @brief Send the current status to ESP32
 */
void SerialProtocol::sendStatus() {
  uint8_t input_states = g_inputs.getAllStates();
  uint8_t output_states = g_outputs.getAllStates();

  plc_link.print("STATE:");
  plc_link.print(input_states, HEX);
  plc_link.print(":");
  plc_link.println(output_states, HEX);
}

/**
 * @brief Send debug message to USB serial only
 */
void SerialProtocol::debugPrint(const char* msg) {
  if (Serial) {
    Serial.println(msg);
  }
}
