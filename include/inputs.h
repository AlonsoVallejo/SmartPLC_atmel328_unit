#ifndef INPUTS_H
#define INPUTS_H

#include <stdint.h>

class Inputs {
public:
  void init();
  void update();
  bool getState(uint8_t index) const;
  uint8_t getAllStates() const;
  bool getRisingEdge(uint8_t index);
  bool getFallingEdge(uint8_t index);

private:
  uint8_t input_state[6];           // Current debounced state
  uint8_t input_state_raw[6];       // Raw input state
  unsigned long debounce_timer[6];  // Debounce timer for each input
  uint8_t previous_state[6];        // Previous state for edge detection
};

extern Inputs g_inputs;

#endif
