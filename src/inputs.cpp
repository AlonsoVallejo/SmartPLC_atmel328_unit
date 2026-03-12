#include <Arduino.h>
#include "inputs.h"
#include "config.h"

Inputs g_inputs;

/*
* @brief Initialize input pins and state
*/
void Inputs::init() {
  /* Initialize all input pins as INPUT (pullups are not used - inputs are optocoupled) */ 
  for (uint8_t i = 0; i < NUM_INPUTS; i++) {
    pinMode(INPUT_PINS[i], INPUT);
    input_state[i] = LOW;
    input_state_raw[i] = LOW;
    debounce_timer[i] = 0;
    previous_state[i] = LOW;
  }
}

/*
* @brief Update input states with debouncing
*/
void Inputs::update() {
  unsigned long current_time = millis();

  for (uint8_t i = 0; i < NUM_INPUTS; i++) {
    uint8_t raw = digitalRead(INPUT_PINS[i]);
    
    /* If raw state differs from debounced state, start debounce timer */
    if (raw != input_state[i]) {
      /* First change detected */
      if (debounce_timer[i] == 0) {
        debounce_timer[i] = current_time;
      }
      /* If debounce time has elapsed, update debounced state */
      else if ((current_time - debounce_timer[i]) >= DEBOUNCE_TIME_MS) {
        input_state[i] = raw;
        debounce_timer[i] = 0;
      }
    } else {
      /* Raw state matches debounced state, reset timer */
      debounce_timer[i] = 0;
    }
  }
}

/**
 * @brief Get state of a specific input
 * @param index Input index (0-5)
 * @return true if input is HIGH, false if LOW
 */
bool Inputs::getState(uint8_t index) const {
  if (index < NUM_INPUTS) {
    return input_state[index] == HIGH;
  }
  return false;
}

/**
 * @brief Get state of all inputs as a byte (bits 0-5 represent inputs 0-5)
 * @return Byte with input states encoded in bits
 */
uint8_t Inputs::getAllStates() const {
  uint8_t states = 0;
  for (uint8_t i = 0; i < NUM_INPUTS; i++) {
    if (input_state[i] == HIGH) {
      states |= (1 << i);
    }
  }
  return states;
}

/**
 * @brief Get rising edge detection for a specific input
 * @param index Input index (0-5)
 * @return true if a rising edge is detected, false otherwise
 */
bool Inputs::getRisingEdge(uint8_t index) {
  if (index < NUM_INPUTS) {
    bool edge = (input_state[index] == HIGH) && (previous_state[index] == LOW);
    previous_state[index] = input_state[index];
    return edge;
  }
  return false;
}

/**
 * @brief Get falling edge detection for a specific input
 * @param index Input index (0-5)
 * @return true if a falling edge is detected, false otherwise
 */
bool Inputs::getFallingEdge(uint8_t index) {
  if (index < NUM_INPUTS) {
    bool edge = (input_state[index] == LOW) && (previous_state[index] == HIGH);
    previous_state[index] = input_state[index];
    return edge;
  }
  return false;
}
