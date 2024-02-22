/**
 * Beispiel für Strings: Benutzereingabe von serieller Konsole einlesen
 * ====================================================================
 *
 * Dieses Beispiel zeigt eine nützliche Anwendung von C-Strings in unserer Vorlesung.
 * Wir bauen eine Funktion, mit der wir Benutzereingaben über die serielle Konsole
 * empfangen können, um beispielsweise nach dem WLAN-Kennwort zu fragen.
 *
 * Gleichzeitig zeigt das Beispiel, wie sich der ESP32 mit einem privaten (geteilter
 * Schlüssel) oder Enterprise-WLAN (Benutzername/Passwort) verbinden kann.
 */

#include <WiFi.h>


String serial_input(const char* prompt) {
  String result = {};

  Serial.print(prompt);

  while (result.length() == 0) {
    result = Serial.readStringUntil('\n');
    result.trim();
  }

  return result;
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  String enterprise_wlan = serial_input("Benutzername/Passwort notwendig? (J/N)\n");
  enterprise_wlan.toUpperCase();  // Schlechtes API-Design: toUpperCase() gibt void zurück!

  if (enterprise_wlan.equals("J")) {
    String ssid = serial_input("WLAN SSID?\n");
    String user = serial_input("WLAN Benutzer?\n");
    String pswd = serial_input("WLAN Passwort?\n");

    WiFi.begin(ssid, WPA2_AUTH_PEAP, user, user, pswd);
  } else {
    String ssid = serial_input("WLAN SSID?\n");
    String psk  = serial_input("WLAN PSK?\n");

    WiFi.begin(ssid, psk);
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nVerbunden!");
}


void loop() {
}