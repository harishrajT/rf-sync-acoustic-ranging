/*
 * Acoustic Ranging — Receiver (ESP32 + nRF24L01+)
 *
 * On receiving the RF "SYNC" packet, starts a microsecond timer and
 * waits for the tone detector output (LM567 in the final build; an
 * STM32-based detector was used in an earlier iteration) to go HIGH.
 *
 * NOTE: the raw flight_time printed here includes the transmitter's
 * fixed 10 ms delay, minus the RF latency (~679 us), plus the tone
 * detector's response delay. These offsets must be calibrated out
 * before the time is converted to an accurate distance. The distance
 * printed below is therefore an uncalibrated value.
 *
 * LM567 NOTE: this loop waits for an active-HIGH detector signal (STM32
 * iteration). The LM567 output is normally active-LOW, so for an LM567-only
 * build change the condition to: while (digitalRead(TONE_DETECTOR_PIN) == HIGH)
 *
 * Wiring: see docs/wiring.md
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);                 // CE = GPIO4, CSN = GPIO5
const byte address[6] = "00001";

#define TONE_DETECTOR_PIN 2       // digital output of the tone detector

unsigned long start_time  = 0;
unsigned long flight_time = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TONE_DETECTOR_PIN, INPUT);

  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1);
  }
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = {0};
    radio.read(&text, sizeof(text));

    if (strcmp(text, "SYNC") == 0) {
      Serial.println("RF Sync received, starting timer...");

      // Step 1: start timer
      start_time = micros();

      // Step 2: wait for the tone detector
      while (digitalRead(TONE_DETECTOR_PIN) == LOW) {
        // waiting for tone
      }

      // Step 3: stop timer
      flight_time = micros() - start_time;

      // Step 4: convert to distance (uncalibrated, see NOTE above)
      float distance_m = (flight_time / 1000000.0) * 343.0;

      Serial.print("Flight time (us): ");
      Serial.print(flight_time);
      Serial.print(" -> Distance: ");
      Serial.print(distance_m, 3);
      Serial.println(" m");
    }
  }
}
