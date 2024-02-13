/**
 * Web File Manager
 * ================
 *
 * Im vorherigen Beispiel haben wir gesehen, wie im internen Flash-Speicher des ESP32 ein
 * FAT-Dateisystem angelegt und benutzt werden kann. Doch wie kommen hier die Dateien hinein,
 * wenn dort beispielsweise statische HTML/CSS/JS-Inhalte für einen Webserver landen sollen,
 * die nicht vom ESP32 selbst erzeugt werden?
 *
 * Für die Arduino IDE 1.x gibt es ein Plugin, das beim Upload der Firmware automatisch auch
 * den Inhalt des Verzeichnisses `data` in das FAT-Dateisystem hochlädt. Bisher hat aber noch
 * niemand dieses Plugin für die Arduino IDE 2.x angepasst. Findige Entwickler haben jedoch
 * Programme geschrieben, die auf dem ESP32 selblst laufen können und dort zum Beispiel einen
 * FTP-Server oder eine Webanwendung zum Verwalten der Dateien bieten.
 *
 * Um genau so ein Projekt handelt es sich hier. Durch Aufspielen dieses Quellcodes auf dem
 * ESP32 wird die vorherige Programmierung temporär ersetzt, um stattdessen den ESP32 im
 * Webbrowser aufrufen und die Inhalte des FAT-Dateisystems pflegen zu können. Danach kann
 * wieder die alte Firmware aufgespielt werden.
 *
 * Benötigte Bibliotheken
 * ----------------------
 *
 * Damit das Beispiel funktioniert, muss im Library Manager der Arduino IDE folgende Bibliothek
 * installiert werden:
 *
 * ESP32 File Manager for Generation Klick ESPFMfGK
 *
 * Danach kann der Quellcode gebaut und hochgeladen werden. Das Programm fragt beim Start nach
 * den WLAN-Zugangsdaten in der seriellen Konsole, so dass diese nicht im Quellcode hinterlegt
 * werden müssen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * Nicht wirklich vorhanden. Am ehesten noch das Git-Repository von ESPFMfGK:
 *
 * https://github.com/holgerlembke/ESPFMfGK/tree/main
 * https://github.com/holgerlembke/ESPFMfGK/blob/main/examples/simple/simple.ino
 */

#include <WiFi.h>


String input(const char* prompt, bool blocking = true);


void setup() {
  Serial.begin(115200);
  delay(1000);

  wifi_connect();

  Serial.println("WLAN ist verbunden!");
}

void loop() {
  // put your main code here, to run repeatedly:

}

// TODO: In wiederverwendbares Include auslagern

/**
 * Abwicklung der WLAN-Verbindung. Gibt true oder false zurück, je nachdem, ob eine
 * Verbindung hergestellt werden konnte.
 */
void wifi_connect() {
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
    String wifi_number_s = input("Bitte die Nummer des zu verwendenden WLANs eingeben: ");
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
        String password = input("Bitte WLAN-Passwort eingeben:\n");
      
        WiFi.begin(ssid, password);
        break;
      }
        
      case WIFI_AUTH_WPA2_ENTERPRISE: {
        String username = input("Bitte Benutzernamen eingeben: ");
        Serial.println(username.c_str());
        String password = input("Bitte Passwort eingeben:\n");

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


/**
 * Hilfsfunktion zum Einlesen einer Textzeile über die serielle Konsole
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