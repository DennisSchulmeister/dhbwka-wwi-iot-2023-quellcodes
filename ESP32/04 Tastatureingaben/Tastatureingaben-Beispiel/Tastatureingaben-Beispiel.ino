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


// Vorwärtsdeklaration der input-Funktion
String input(const char* prompt, bool blocking = true);


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
  String name = input("Wie heißt du? ");
  Serial.printf("\nHallo %s!\n\n", name);
}


/**
 * Hilfsfunktion zum Einlesen einer Zeile über den USB Serial Port.
 * In der Arduino IDE muss die Zeile in das Eingabefeld ganz oben eingegen werden.
 *
 * Die Funktion blockiert so lange, bis eine komplette Zeile empfangen wurde.
 * Falls dies nicht gewünscht ist, muss der zweite Parameter mit false belegt werden.
 * In diesem Fall kann mit Serial.setTimeout() ein Timeout festgelegt werden, nachem
 * die Funktion automatisch einen leeren String zurückliefert, wenn keine Eingabe
 * empfangen wurde.
 */
String input(const char* prompt, bool blocking) {
  String result = {};

  Serial.print(prompt);

  while (result.length() == 0) {
    result = Serial.readStringUntil('\n');
    result.trim();

    if (!blocking) break;
  }

  return result;
}