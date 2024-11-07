/**
 * Software Serial: Sender
 * =======================
 *
 * Dies ist eine Minimalbeispiel dafür, wie einfach über eine serielle Leitung Daten übertragen
 * werden können. Der der Arduino Uno (Atmega 328p) allerdings nur einen Hardware UART besitzt,
 * der darüber hinaus auch fest mit dem USB-Anschluss (und damit der seriellen Konsole der
 * Arduino IDE) verbunden ist, wird die serielle Datenübertragen mit der eingebauten Bibliothek
 * `SoftwareSerial` in Software realisiert.
 *
 * Dieses Programm zählt einfach jede Sekunde eine Zahl hoch und sendet sie über die serielle
 * Schnittstelle an einen zweiten Arduino.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *   +-----------------------+         +-----------------------+
 *   |                   GND |<------->| GND                   |
 *   | Sendender Arduino     |         |  Empfangender Arduino |
 *   |            Pin 3 (TX) |-------->| PIN 2 (RX)            |
 *   +-----------------------+         +-----------------------+
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/communication/Serial/
 * https://docs.arduino.cc/learn/built-in-libraries/software-serial/
 */

#include <SoftwareSerial.h>

constexpr int software_serial_rx = 2;
constexpr int software_serial_tx = 3;

SoftwareSerial software_serial(software_serial_rx, software_serial_tx);

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  while (!Serial);

  Serial.begin(9600);
  Serial.println("Bereit ...");

  software_serial.begin(9600);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int counter = -1;
  counter++;

  Serial.print("Sende: ");
  Serial.println(counter);

  software_serial.println(counter);

  delay(1000);
}
