# Wiring

Both ESP32 boards connect to their nRF24L01+ module the same way.

| nRF24L01+ | ESP32 | Notes |
|---|---|---|
| GND | GND | |
| VCC | 3V3 | 3.3 V only — 5 V damages the module |
| CE | GPIO 4 | Chip enable |
| CSN | GPIO 5 | SPI chip select |
| SCK | GPIO 18 | SPI clock |
| MOSI | GPIO 23 | |
| MISO | GPIO 19 | |
| IRQ | — | Not connected |

## GPIO 2 usage

| Sketch | Transmitter GPIO 2 | Receiver GPIO 2 |
|---|---|---|
| `latency-test` | Wired reference pulse out | Wired reference pulse in (interrupt, falling edge) |
| `ranging` | Gates the tone generator / LM386 (ASK on/off) | LM567 tone detector output |

For the latency test, connect the two boards' GND together as well as GPIO 2.
