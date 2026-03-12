#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <stdint.h>

class Outputs {
public:

  void init();
  void setOn(uint8_t index);
  void setOff(uint8_t index);
  void toggle(uint8_t index);
  bool getState(uint8_t index) const;
  uint8_t getAllStates() const;
  void setAllStates(uint8_t states);
  void allOff();

private:
  uint8_t output_state[6]; 
};

extern Outputs g_outputs;

#endif
