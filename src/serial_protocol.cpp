#include <Arduino.h>
#include <string.h>
#include "serial_protocol.h"
#include "config.h"
#include "inputs.h"
#include "outputs.h"

SerialProtocol g_serial;

/*
* @brief Initialize serial communication and internal state
*/
void SerialProtocol::init() {
  Serial.begin(SERIAL_BAUD);
  rx_index = 0;
  memset(rx_buffer, 0, RX_BUFFER_SIZE);
}

/**
 * @brief Update serial communication and process incoming commands
 * @return true if a command was processed, false otherwise
 */
bool SerialProtocol::update() {
  bool command_processed = false;

  while (Serial.available() > 0) {
    char c = Serial.read();

    /* Handle different terminators */ 
    if (c == '\n' || c == '\r') {
      if (rx_index > 0) {
        rx_buffer[rx_index] = '\0';
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
 * @brief Process a received command
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
    /* Unknown command - echo for debugging */
    Serial.print("ERR:Unknown command: ");
    Serial.println(command);
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
    Serial.println("ERR:Invalid output command");
    return false;
  }

  /* Convert hex string to byte value */
  unsigned int temp_value = 0;
  if (sscanf(params, "%02x", &temp_value) == 1) {
    /* Apply only the 6 valid output bits (mask 0x3F) */
    uint8_t output_value = (uint8_t)(temp_value & 0x3F);
    g_outputs.setAllStates(output_value);
    // Send status response after command is processed
    sendStatus();
    return true;
  } else {
    Serial.println("ERR:Invalid hex value");
    return false;
  }
}

/**
 * @brief Send the current status over serial
 */
void SerialProtocol::sendStatus() {
  uint8_t input_states = g_inputs.getAllStates();
  uint8_t output_states = g_outputs.getAllStates();

  Serial.print("STATE:");
  Serial.print(input_states, HEX);
  Serial.print(":");
  Serial.println(output_states, HEX);
}
