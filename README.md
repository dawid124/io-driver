# IO Driver

Firmware for a smart home I/O driver node based on the **Raspberry Pi Pico (RP2040)**. Each node connects via Ethernet (ENC28J60) and communicates with a central MQTT broker to receive commands and publish sensor events.

## Hardware

- **MCU**: Raspberry Pi Pico (RP2040, dual-core @ 133 MHz)
- **Network**: ENC28J60 Ethernet module (SPI, CS on pin 17)
- **Outputs**: Switches, blinds (relay pairs), RGB/RGBW lights, NeoPixel LED strips
- **Inputs**: PIR motion sensors

## Architecture

The firmware uses both RP2040 cores:

- **Core 0** — MQTT communication, motion sensor polling, action dispatch
- **Core 1** — Device servicing (lights, blinds, switches)

Actions received over MQTT are queued and dispatched to the appropriate core's device stack.

## MQTT Topics

| Topic | Direction | Purpose |
|---|---|---|
| `home/driver-sync` | Subscribe | Device configuration / initial state sync |
| `home/actions` | Subscribe | Incoming actions (switch, dim, color, etc.) |
| `home/triggers` | Publish | Motion sensor state changes |

## Supported Device Types

- `Switch` — digital on/off output
- `Blind` — motor control (open/close/stop)
- `RgbLight` / `RgbwLight` — PWM RGB(W) lighting
- `NeoLight` — NeoPixel (WS2812) LED strips
- `MotionSensor` — PIR input with state publishing

## Configuration

Each node is identified at build time via `platformio.ini` build flags:

```ini
-D DEVICE_NAME="IO-DRIVER-F0-1"
-D DEVICE_MAC="BE-88-1A-D3-FE-F2"
-D DEVICE_IP="192.168.0.57"
-D MQTT_SERVER="pi-master"
```

Uncomment the desired device block and flash.

## Dependencies

| Library | Version |
|---|---|
| EthernetENC | ~2.0.3 |
| PubSubClient | 2.8 |
| ArduinoJson | 6.19.4 |
| Adafruit NeoPixel | 1.10.5 |

## Build & Flash

Requires [PlatformIO](https://platformio.org/).

```bash
pio run --target upload
```

Serial monitor at 115200 baud (`COM6` by default, configurable in `platformio.ini`).
