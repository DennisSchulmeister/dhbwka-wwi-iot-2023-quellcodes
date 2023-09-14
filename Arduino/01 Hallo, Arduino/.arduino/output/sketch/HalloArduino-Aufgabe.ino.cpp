#include <Arduino.h>
#line 1 "/home/dennis/Documents/DHBW-Karlsruhe/# Wahlmodul IoT/Quellcodes/Arduino/01 Hallo, Arduino/HalloArduino-Aufgabe/HalloArduino-Aufgabe.ino"
/**
 * Hallo, Welt für Arduino
 * =======================
 *
 * Konfiguriert alle GPIO-Pins als digitale Ausgänge und schaltet diese
 * jede Sekunde ein und wieder aus. Dadurch blinkt die eingebaute LED des
 * Arduino sowie jedes angeschlossene Bauteil. Zusätzlich wird eine Meldung
 * auf der seriellen Konsole ausgegeben.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Keine Besonderheiten. Zum Testen kann eine LED oder Buzzer an einen der
 * Ausgänge gehängt werden.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
 * https://www.arduino.cc/reference/en/language/functions/communication/serial/
 */

// Konstanten und globale Variablen
const int MIN_PIN = 0;
const int MAX_PIN = 19;

bool output = false;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
#line 33 "/home/dennis/Documents/DHBW-Karlsruhe/# Wahlmodul IoT/Quellcodes/Arduino/01 Hallo, Arduino/HalloArduino-Aufgabe/HalloArduino-Aufgabe.ino"
void setup();
#line 48 "/home/dennis/Documents/DHBW-Karlsruhe/# Wahlmodul IoT/Quellcodes/Arduino/01 Hallo, Arduino/HalloArduino-Aufgabe/HalloArduino-Aufgabe.ino"
void loop();
#line 33 "/home/dennis/Documents/DHBW-Karlsruhe/# Wahlmodul IoT/Quellcodes/Arduino/01 Hallo, Arduino/HalloArduino-Aufgabe/HalloArduino-Aufgabe.ino"
void setup() {
  // Alle GPIO-Pins als digitale Ausgänge konfigurieren
  for (int i = MIN_PIN; i <= MAX_PIN; i++) {
    pinMode(i, OUTPUT);
  }

  // Serielle Kommunikation mit dem Host-PC initialisieren
  Serial.begin(9600);
  Serial.println();
  Serial.println("Der Arduino sagt hallo!");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  Serial.println(output ? "Schalte alle Pins an." : "Schalte alle Pins aus.");

  for (int i = MIN_PIN; i <= MAX_PIN; i++) {
    digitalWrite(i, output);
  }

  output = !output;
  delay(1000);
}

