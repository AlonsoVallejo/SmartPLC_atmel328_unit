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
#define SERIAL_BAUD  9600
#define SERIAL_TIMEOUT_MS 100

// ============================================================================
// INPUT DEBOUNCE SETTINGS
// ============================================================================
#define DEBOUNCE_TIME_MS 20  // Debounce time for input state changes

// ============================================================================
// TIMING SETTINGS
// ============================================================================
#define MAIN_LOOP_RATE_MS 50  // Main loop cycle time (20 Hz)
#define HEARTBEAT_RATE_MS 500 // Heartbeat/status update interval

#endif
