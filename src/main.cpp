#include <Arduino.h>
#include "config.h"
#include "inputs.h"
#include "outputs.h"
#include "serial_protocol.h"

unsigned long last_update_time = 0;

void setup() {
  /* Initialize dual serial ports */
  g_serial.init();
  
  delay(100); 
  
  /* Debug messages to USB serial only */
  Serial.println("=== SmartPLC Arduino Nano - Sensor/Actuator Unit ===");
  Serial.println("Hardware: Arduino Nano ATmega328p");
  
  /* Initialize I/O modules */
  g_inputs.init();
  g_outputs.init();
  
  Serial.println("✓ Inputs initialized (D8, D9, D10, D11, D12, D14)");
  Serial.println("✓ Outputs initialized (D2-D7)");
  Serial.println("✓ PLC link ready on A1/A2 (9600 baud to ESP32)");
  Serial.println("✓ Debug ready on USB (115200 baud)");
  Serial.println("Waiting for ESP32 commands...");
  
  last_update_time = millis();
}

void loop() {
  unsigned long current_time = millis();

  /* ========== Serial communication (high priority) ========== */
  /* Process incoming commands as often as possible */
  g_serial.update();

  /* ========== Update cycle (MAIN_LOOP_RATE_MS) ========== */
  if ((current_time - last_update_time) >= MAIN_LOOP_RATE_MS) {
    last_update_time = current_time;
    
    /* Update input states (handles debouncing) */
    g_inputs.update();
  }
}
