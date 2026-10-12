/*
 * RF Latency Test — Receiver (ESP32 + nRF24L01+)
 *
 * Timestamps the wired reference pulse in an interrupt (falling edge),
 * then timestamps the arrival of the RF packet. Prints
 *   (t_packet - t_pulse) - 10000 us
 * which is the RF latency in microseconds.
 *
 * Wiring: see docs/wiring.md
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN    4
#define CSN_PIN   5
#define INPUT_PIN 2   // wired reference pulse from the transmitter

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

volatile unsigned long t_pulse = 0;
volatile bool pulseReceived = false;

void IRAM_ATTR handlePulse() {
  t_pulse = micros();
  pulseReceived = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(INPUT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), handlePulse, FALLING);

  if (!radio.begin()) {
    Serial.println("nRF24L01 not responding");
    while (1);
  }
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text;
    radio.read(&text, sizeof(text));
    unsigned long t_packet = micros();

    if (pulseReceived) {
      unsigned long delayTime = t_packet - t_pulse;
      Serial.println(delayTime - 10000);   // RF latency in us
      pulseReceived = false;               // reset for next measurement
    }
  }
}
