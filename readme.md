# SmartPLC Arduino Nano Architecture

## Project Overview
- Arduino Nano 328p based PLC controller
- 6 optocoupled inputs (D8, D9, D10, D11, D12, D14)
- 6 relay outputs (D2-D7)
- Serial communication with ESP32 control unit (115200 baud)
- Non-blocking, modular architecture

## Modular Components Created

### 1. config.h
Central configuration file with:
- Pin definitions (inputs & outputs)
- Serial settings
- Debounce and timing constants

### 2. Input Module (inputs.h/cpp)
Features:
- Debounce management (20ms default)
- Edge detection (rising/falling)
- Individual state queries
- Batch state reporting (6 bits in 1 byte)

Public API:
- `init()` - Initialize input pins
- `update()` - Update states (call regularly)
- `getState(index)` - Get single input state
- `getAllStates()` - Get all inputs as byte
- `getRisingEdge(index)` - Detect LOW→HIGH transition
- `getFallingEdge(index)` - Detect HIGH→LOW transition

### 3. Output Module (outputs.h/cpp)
Features:
- Individual relay control
- Batch control (all at once)
- State tracking

Public API:
- `init()` - Initialize output pins
- `setOn(index)` / `setOff(index)` - Control individual relays
- `toggle(index)` - Toggle relay state
- `getState(index)` - Get single output state
- `getAllStates()` - Get all outputs as byte
- `setAllStates(byte)` - Set all outputs from byte
- `allOff()` - Emergency stop (all relays off)

### 4. Serial Protocol (serial_protocol.h/cpp)
Simple text-based protocol:

Commands from ESP32:
- `OUT:xx` - Set outputs (xx = 2 hex digits for bits 0-5)
- `REQ` - Request current state

Responses to ESP32:
- `STATE:in:xx` - Input states and output states (hex format)

### 5. Main Application (main.cpp)
Non-blocking architecture with timing-based updates:
- 50ms input update cycle (20 Hz)
- 500ms heartbeat status broadcast
- Serial communication processed continuously
- Empty application logic section for user code

## Design Principles
✓ Non-blocking (no delays in main loop)
✓ Modular (independent components)
✓ No RTOS required
✓ Memory efficient for Arduino Nano
✓ Extensible (easy to add new features)

## Build & Deploy
- PlatformIO configured for Arduino Nano 328p
- Framework: Arduino
- Ready to build: `platformio run`
- Ready to upload: `platformio run --target upload`
