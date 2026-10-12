/*
 * Acoustic Ranging — Transmitter (ESP32 + nRF24L01+)
 *
 * Each cycle:
 *   1. Send an RF "SYNC" packet (time zero for the receiver).
 *   2. Wait a fixed 10 ms.
 *   3. Switch ASK_PIN high for 100 ms. This gates an external tone
 *      generator / LM386 amplifier, producing an on-off keyed (ASK)
 *      sound burst from the speaker.
 *
 * Wiring: see docs/wiring.md
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);                 // CE = GPIO4, CSN = GPIO5
const byte address[6] = "00001";

#define ASK_PIN 2                 // gates the external tone generator (ASK on/off); 16 kHz in the final build

void setup() {
  pinMode(ASK_PIN, OUTPUT);
  digitalWrite(ASK_PIN, LOW);
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1);
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {
  // Step 1: RF sync packet
  const char text[] = "SYNC";
  radio.write(&text, sizeof(text));
  Serial.println("RF Sync sent");

  // Step 2: fixed 10 ms delay before the sound burst
  delay(10);

  // Step 3: 100 ms ASK sound burst
  digitalWrite(ASK_PIN, HIGH);
  delay(100);
  digitalWrite(ASK_PIN, LOW);
  Serial.println("Sound pulse sent");

  delay(1000);                    // repeat every ~1 s
}
