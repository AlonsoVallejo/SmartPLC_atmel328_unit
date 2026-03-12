#include <Arduino.h>
#include "config.h"
#include "inputs.h"
#include "outputs.h"
#include "serial_protocol.h"

unsigned long last_update_time = 0;

void setup() {
  g_serial.init();
  
  delay(100); 
  
  Serial.println("=== SmartPLC Arduino Nano Initializing ===");
  
  g_inputs.init();
  g_outputs.init();
  
  Serial.println("Inputs initialized (D8, D9, D10, D11, D12, D14)");
  Serial.println("Outputs initialized (D2-D7)");
  Serial.println("Ready for communication");
  
  last_update_time = millis();
}

void loop() {

  /* ========== Serial communication (high priority) ========== */
  /* Process incoming commands as often as possible */
  g_serial.update();
}
