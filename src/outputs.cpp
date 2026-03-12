#include <Arduino.h>
#include "outputs.h"
#include "config.h"

Outputs g_outputs;

/**
 * Initialize output pins
 */
void Outputs::init() {
  /*  Initialize all output pins as OUTPUT set to LOW (relays OFF) */
  for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
    pinMode(OUTPUT_PINS[i], OUTPUT);
    digitalWrite(OUTPUT_PINS[i], LOW);
    output_state[i] = LOW;
  }
}

/**
 * @brief Set a specific output HIGH (relay ON)
 * @param index Output index (0-5)
 */
void Outputs::setOn(uint8_t index) {
  if (index < NUM_OUTPUTS) {
    output_state[index] = HIGH;
    digitalWrite(OUTPUT_PINS[index], HIGH);
  }
}

/**
 * @brief Set a specific output LOW (relay OFF)
 * @param index Output index (0-5)
 */
void Outputs::setOff(uint8_t index) {
  if (index < NUM_OUTPUTS) {
    output_state[index] = LOW;
    digitalWrite(OUTPUT_PINS[index], LOW);
  }
}

/**
 * @brief Toggle a specific output
 * @param index Output index (0-5)
 */
void Outputs::toggle(uint8_t index) {
  if (index < NUM_OUTPUTS) {
    output_state[index] = (output_state[index] == HIGH) ? LOW : HIGH;
    digitalWrite(OUTPUT_PINS[index], output_state[index]);
  }
}

/**
 * @brief Get state of a specific output
 * @param index Output index (0-5)
 * @return true if output is HIGH, false if LOW
 */
bool Outputs::getState(uint8_t index) const {
  if (index < NUM_OUTPUTS) {
    return output_state[index] == HIGH;
  }
  return false;
}

/**
 * @brief Get state of all outputs as a byte (bits 0-5 represent outputs 0-5)
 * @return Byte with output states encoded in bits
 */
uint8_t Outputs::getAllStates() const {
  uint8_t states = 0;
  for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
    if (output_state[i] == HIGH) {
      states |= (1 << i);
    }
  }
  return states;
}

/**
 * @brief Set states of all outputs using a byte (bits 0-5 represent outputs 0-5)
 * @param states Byte with desired output states encoded in bits
 */
void Outputs::setAllStates(uint8_t states) {
  for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
    if (states & (1 << i)) {
      setOn(i);
    } else {
      setOff(i);
    }
  }
}

/**
 * @brief Turn all outputs OFF
 */
void Outputs::allOff() {
  for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
    setOff(i);
  }
}
