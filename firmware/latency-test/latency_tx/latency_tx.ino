/*
 * RF Latency Test — Transmitter (ESP32 + nRF24L01+)
 *
 * Purpose: measure how long an nRF24L01+ packet takes to arrive, so the
 * delay can be subtracted from acoustic time-of-flight readings.
 *
 * Method: a wired reference pulse (GPIO 2 -> receiver GPIO 2) marks a
 * known instant. 10 ms later an RF packet is sent. The receiver times
 * the gap between the wired pulse and the RF packet; anything above
 * 10 ms is the RF latency.
 *
 * Wiring: see docs/wiring.md
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN    4
#define CSN_PIN   5
#define PULSE_PIN 2   // wired reference pulse to the receiver

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, LOW);
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println("nRF24L01 not responding");
    while (1);
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  // Step 1: wired reference pulse (receiver timestamps the falling edge)
  digitalWrite(PULSE_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(PULSE_PIN, LOW);

  // Step 2: fixed 10 ms gap
  delay(10);

  // Step 3: send RF packet
  const char text = 'p';
  radio.write(&text, sizeof(text));
  Serial.println("Packet Sent");

  delay(100);
}
