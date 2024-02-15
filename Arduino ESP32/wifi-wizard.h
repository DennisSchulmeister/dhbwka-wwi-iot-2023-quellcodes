#pragma once

#include <WiFi.h>
#include "serial-input.h"

/**
 * Abwicklung der WLAN-Verbindung. Gibt true oder false zurück, je nachdem, ob eine
 * Verbindung hergestellt werden konnte.
 */
void wifi_wizard() {
  bool connected = false;

  while (!connected) {
    // Vorhandene WLAN-Netze anzeigen
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    size_t n_wifi = WiFi.scanNetworks();

    if (n_wifi == 0) {
      Serial.println("Kein WLAN gefunden.");
      delay(2000);
      continue;
    }

    Serial.printf("%u WLAN-Netze gefunden:\n\n", n_wifi);

    for (int i = 0; i < n_wifi; i++) {
      Serial.printf("[%u]  ", i);
      Serial.printf("%.32s  ", WiFi.SSID(i).c_str());
      Serial.printf("(%lu)  ", WiFi.RSSI(i));

      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:
          Serial.println("OPEN");
          break;
        case WIFI_AUTH_WEP:
          Serial.println("WEP");
          break;
        case WIFI_AUTH_WPA_PSK:
          Serial.println("WPA_PSK");
          break;
        case WIFI_AUTH_WPA2_PSK:
          Serial.println("WPA2_PSK");
          break;
        case WIFI_AUTH_WPA_WPA2_PSK:
          Serial.println("WPA_WPA2_PSK");
          break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
          Serial.println("WPA2_ENTERPRISE");
          break;
        default:
          Serial.println();
      }
    }

    Serial.println();
    String wifi_number_s = serial_input("Bitte die Nummer des zu verwendenden WLANs eingeben: ");
    Serial.printf("%s\n", wifi_number_s.c_str());

    int wifi_number_i = atoi(wifi_number_s.c_str());
    String ssid = WiFi.SSID(wifi_number_i);

    if (wifi_number_i < 0 || wifi_number_i > n_wifi) {
      Serial.println("Ungültige Eingabe!\n");
      continue;
    }

    // Verbindung zu ausgewähltem Netz herstellen
    Serial.printf("Stelle Verbindung her zu %s\n", ssid.c_str());

    switch (WiFi.encryptionType(wifi_number_i)) {
      case WIFI_AUTH_OPEN:
        WiFi.begin(ssid);
        break;

      case WIFI_AUTH_WPA_PSK:
      case WIFI_AUTH_WPA2_PSK:
      case WIFI_AUTH_WPA_WPA2_PSK: {
        String password = serial_input("Bitte WLAN-Passwort eingeben:\n");

        WiFi.begin(ssid, password);
        break;
      }

      case WIFI_AUTH_WPA2_ENTERPRISE: {
        String username = serial_input("Bitte Benutzernamen eingeben: ");
        Serial.println(username.c_str());
        String password = serial_input("Bitte Passwort eingeben:\n");

        WiFi.begin(ssid, WPA2_AUTH_PEAP, username, username, password);
        break;
      }

      default:
        Serial.println("Authentifizierungsmethode nicht unterstützt. Bitte anderes WLAN auswählen!\n");
        continue;
    }

    for (int i = 0; i < 15; i++) {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
      } else {
        connected = true;
      }
    }

    Serial.println();

    if (!connected) {
      Serial.println("Verbindung fehlgeschlagen. Bitte versuchen Sie es erneut.\n");
    }
  }
}