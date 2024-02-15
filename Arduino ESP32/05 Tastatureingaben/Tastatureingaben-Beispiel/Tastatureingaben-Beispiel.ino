/**
 * Tastatureingaben über den USB Serial Port
 * =========================================
 *
 * Dieses Beispiel ist eigentlich nicht ESP32-spezifisch, sondern zeigt, wie mit der Arduino
 * Serial Library Tastatuseingaben vom Host-Computer eingelesen werden können. Im Zusammenhang
 * mit dem ESP32 kann dies ganz nützlich sein, um die WLAN-Zugangsdaten nicht in den Quellcode
 * eintragen zu müssen, sondern beim ersten Start abzufragen, ohne hierfür gleich einen WiFi
 * Access Point mit HTTP-Webserver und Konfigurations-UI programmieren zu müssen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * Serial.readStringUntil(): https://www.arduino.cc/reference/en/language/functions/communication/serial/readstringuntil/
 * Serial.setTimeout(): https://www.arduino.cc/reference/en/language/functions/communication/serial/settimeout/
 * String: https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/
 */

#include "serial-input.h"


/**
 * Initialisierung bei Neustart des Microcontrollers
 */
void setup() {
  Serial.begin(115200);
}


/**
 * Unenddlich auf ausgeführte Programmlogik
 */
void loop() {
  String name = serial_input("Wie heißt du? ");
  Serial.printf("\nHallo %s!\n\n", name.c_str());

  // WICHTIG: Zur Ausgabe des Strings mit Serial.printX() muss das String-Objekt
  // mit der c_str()-Methode in ein null-terminiertes Byte-Array umgewandelt werden
  // (String der Programmiersprache C)!
}
