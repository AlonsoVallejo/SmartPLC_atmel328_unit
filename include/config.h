#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// HARDWARE PIN DEFINITIONS
// ============================================================================

// Digital Input Pins (from optocoupled inputs)
#define INPUT_PIN_0  8
#define INPUT_PIN_1  9
#define INPUT_PIN_2  10
#define INPUT_PIN_3  11
#define INPUT_PIN_4  12
#define INPUT_PIN_5  14

#define NUM_INPUTS   6
static const uint8_t INPUT_PINS[NUM_INPUTS] = {
  INPUT_PIN_0, INPUT_PIN_1, INPUT_PIN_2, 
  INPUT_PIN_3, INPUT_PIN_4, INPUT_PIN_5
};

// Digital Output Pins (to relays)
#define OUTPUT_PIN_0 2
#define OUTPUT_PIN_1 3
#define OUTPUT_PIN_2 4
#define OUTPUT_PIN_3 5
#define OUTPUT_PIN_4 6
#define OUTPUT_PIN_5 7

#define NUM_OUTPUTS  6
static const uint8_t OUTPUT_PINS[NUM_OUTPUTS] = {
  OUTPUT_PIN_0, OUTPUT_PIN_1, OUTPUT_PIN_2,
  OUTPUT_PIN_3, OUTPUT_PIN_4, OUTPUT_PIN_5
};

// ============================================================================
// SERIAL COMMUNICATION SETTINGS
// ============================================================================
// Hardware Serial (USB for debugging/programming)
#define DEBUG_SERIAL_BAUD 115200

// SoftwareSerial (ESP32 PLC link)
// Uses pins A1 (RX from ESP32) and A2 (TX to ESP32)
#define PLC_SERIAL_RX_PIN A1  // Pin 15 - receive from ESP32 TX
#define PLC_SERIAL_TX_PIN A2  // Pin 16 - transmit to ESP32 RX
#define PLC_SERIAL_BAUD 9600  // Match ESP32 Serial2 configuration

// ============================================================================
// INPUT DEBOUNCE SETTINGS
// ============================================================================
#define DEBOUNCE_TIME_MS 20  // Debounce time for input state changes

// ============================================================================
// TIMING SETTINGS
// ============================================================================
#define MAIN_LOOP_RATE_MS 50  // Main loop cycle time (20 Hz)

#endif
