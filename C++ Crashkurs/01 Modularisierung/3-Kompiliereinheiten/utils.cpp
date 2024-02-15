#include <Arduino.h>

/**
 * Implementierung der Funktion sagHallo()
 */
void sagHallo() {
  Serial.begin(115200);
  Serial.println("Hallo aus der Datei utils.cpp");
}