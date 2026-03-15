# SmartPLC Arduino Nano - Sensor/Actuator Unit

**Part of the SmartPLC System** | [ESP32 Touchscreen Control Unit](https://github.com/AlonsoVallejo/SmartPLC_esp32_touchscreen)

## Project Overview
Industrial-grade PLC sensor and actuator I/O unit based on Arduino Nano ATmega328p. This project implements the remote I/O module that communicates with an ESP32-based control unit over a serial link. The architecture is modular, non-blocking, and requires no RTOS.

### Hardware Specifications
- **Microcontroller**: Arduino Nano ATmega328p (16 MHz, 2KB RAM)
- **Inputs**: 6 optocoupled digital inputs (D8, D9, D10, D11, D12, D14)
- **Outputs**: 6 relay digital outputs (D2-D7)
- **Communication**: 
  - ESP32 Link: SoftwareSerial on A1 (RX), A2 (TX) @ 9600 baud
  - USB Debug: Hardware Serial @ 115200 baud
- **Power**: USB or external 5V supply

## Dual Serial Architecture Explanation

This project uses **two independent serial ports** to eliminate UART contention:

| Serial Link | Port | Pins | Baud | Purpose |
|---|---|---|---|---|
| **PLC Link** | SoftwareSerial | A1 (RX), A2 (TX) | 9600 | Communication with ESP32 control unit |
| **Debug Link** | Hardware Serial | D0/D1 + USB | 115200 | Programming & debug output (isolated from PLC link) |

This separation ensures:
- ✓ No data corruption from USB/ESP32 contention
- ✓ USB cable can stay connected during ESP32 operation
- ✓ Clean debug output while PLC link is active
- ✓ Safe for development and production

## Modular Architecture

### 1. config.h
Central configuration repository:
- Pin definitions (inputs, outputs, serial)
- Serial communication settings (baud rates)
- Debounce and timing parameters
- Easily customizable for different board configurations

### 2. Input Module (`inputs.h` / `inputs.cpp`)
Manages 6 digital inputs with automatic debouncing:
- **Features**:
  - Hardware debounce (20ms configurable)
  - Rising/falling edge detection
  - Batch state reporting (6 bits in 1 byte)
  - Non-blocking state machine

- **Public API**:
  ```cpp
  g_inputs.init()                    // Initialize pins
  g_inputs.update()                  // Update states (call ~20Hz)
  g_inputs.getState(index)           // Get single input (0-5)
  g_inputs.getAllStates()            // Get all inputs as byte (1 byte = 6 bits)
  g_inputs.getRisingEdge(index)      // Detect LOW→HIGH transition
  g_inputs.getFallingEdge(index)     // Detect HIGH→LOW transition
  ```

### 3. Output Module (`outputs.h` / `outputs.cpp`)
Controls 6 relay outputs with persistent state tracking:
- **Features**:
  - Individual relay control
  - Batch control (set all 6 at once)
  - State persistence
  - Emergency stop function

- **Public API**:
  ```cpp
  g_outputs.init()                   // Initialize pins
  g_outputs.setOn(index)             // Turn on relay (0-5)
  g_outputs.setOff(index)            // Turn off relay
  g_outputs.toggle(index)            // Toggle relay state
  g_outputs.getState(index)          // Get single output state
  g_outputs.getAllStates()           // Get all outputs as byte
  g_outputs.setAllStates(byte)       // Set all outputs from byte
  g_outputs.allOff()                 // Emergency stop (all OFF)
  ```

### 4. Serial Protocol Module (`serial_protocol.h` / `serial_protocol.cpp`)
Handles ESP32 communication over SoftwareSerial:
- **Protocol**: Simple text-based, newline-terminated
- **Commands Received from ESP32**:
  - `OUT:xx` - Set outputs (xx = 2 hex digits, bits 0-5 for outputs 0-5)
  - `REQ` - Request current state

- **Responses Sent to ESP32**:
  - `STATE:in:xx` - Response with input and output states (hex format)

- **Example dialogue**:
  ```
  ESP32 → Arduino: OUT:2F       (set outputs 0,1,2,3,5 ON)
  Arduino → ESP32: STATE:1A:2F  (inputs=0x1A, outputs=0x2F)
  
  ESP32 → Arduino: REQ
  Arduino → ESP32: STATE:1A:2F
  ```

### 5. Main Application (`main.cpp`)
Non-blocking event loop architecture:
- **Timing-based updates**:
  - ~50ms input sampling cycle (handles debouncing)
  - Continuous serial communication polling
- **No blocking delays** in main loop
- **Extensible**: Add application logic between serial updates

## Build & Deploy

### Requirements
- [PlatformIO](https://platformio.org/platformio-ide) with Arduino extension
- Arduino Nano ATmega328p board

### Build
```bash
platformio run
```

### Upload
```bash
platformio run --target upload
```

### Monitor
```bash
platformio device monitor --baud 115200
```

## Wiring Diagram

```
Arduino Nano (ATmega328p)
┌─────────────────────────────────────────┐
│                                           │
│  INPUTS (Optocoupled)    OUTPUTS (Relays)│
│  D8  ← INPUT_0           D2  → RELAY_0   │
│  D9  ← INPUT_1           D3  → RELAY_1   │
│  D10 ← INPUT_2           D4  → RELAY_2   │
│  D11 ← INPUT_3           D5  → RELAY_3   │
│  D12 ← INPUT_4           D6  → RELAY_4   │
│  D14 ← INPUT_5           D7  → RELAY_5   │
│                                           │
│  SERIAL LINKS                             │
│  D0/D1 ─→ USB (Debug, 115200 baud)      │
│  A1/A2 ─→ ESP32 (PLC link, 9600 baud)   │
│                                           │
└─────────────────────────────────────────┘
```

### ESP32 Connection (SoftwareSerial)
```
Arduino Nano A1 (RX) ← ESP32 GPIO17 (TX) [with resistor divider]
Arduino Nano A2 (TX) → ESP32 GPIO16 (RX) [optional resistor for 5V→3.3V]
Arduino Nano GND     ← ESP32 GND
```

**⚠️ Important**: Use a voltage divider or logic level shifter on Arduino TX → ESP32 RX 
(Nano TX is 5V, ESP32 RX is 3.3V max)

## Integration with ESP32 Control Unit

This Sensor/Actuator unit communicates with the **SmartPLC ESP32 Touchscreen** project:

- **Repository**: [SmartPLC_esp32_touchscreen](https://github.com/AlonsoVallejo/SmartPLC_esp32_touchscreen)
- **ESP32 configures**: 
  ```cpp
  #define PLC_SERIAL Serial2
  #define PLC_BAUD_RATE 9600
  #define PLC_RX_PIN 16  // from Arduino A2
  #define PLC_TX_PIN 17  // to Arduino A1
  ```

The ESP32 project sends `OUT:` commands to control relays and polls `REQ` to read sensor states, displaying everything on a touchscreen interface.

## Memory Usage

Compiled for Arduino Nano:
- **Flash**: ~6.5 KB (available: 30 KB)
- **RAM**: ~800 bytes (available: 2 KB)

Very efficient non-blocking architecture suitable for resource-constrained embedded systems.

## Non-Blocking Design Principles

✓ No `delay()` in main loop  
✓ Timing-based state machines  
✓ Responsive serial communication  
✓ No RTOS overhead  
✓ Deterministic timing (~50ms input cycle)  

## Application Customization

Add your control logic in `main.cpp`:

```cpp
// Example: Mirror input 0 to output 0
if (g_inputs.getState(0)) {
  g_outputs.setOn(0);
} else {
  g_outputs.setOff(0);
}

// Example: Toggle output on rising edge of input 1
if (g_inputs.getRisingEdge(1)) {
  g_outputs.toggle(1);
}

// Example: Emergency stop on any input
if (g_inputs.getRisingEdge(2)) {
  g_outputs.allOff();
}
```

## License & Credits

Designed as part of the SmartPLC industrial automation system.
For questions or contributions, refer to the main [SmartPLC_esp32_touchscreen](https://github.com/AlonsoVallejo/SmartPLC_esp32_touchscreen) repository.
