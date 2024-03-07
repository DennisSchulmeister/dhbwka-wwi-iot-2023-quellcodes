/**
 * ThingsBoard IoT-Backend
 * =======================
 *
 * Dieses Beispiel zeigt, wie der ESP32 über das MQTT-Protokoll mit einem ThingsBoard-Server
 * kommunizieren kann. Hierfür beinhaltet die Datei `thingsboard.h` eine selbst-geschriebene
 * Klasse, welche die Kommunikation abwickelt. Sie stellt folgende Funktionen zur Verfügung:
 *
 *  - Senden von Telemetriedaten
 *  - Senden von Attributen
 *  - Senden von RPC-Anfragen
 *  - Empfangen von Attributen
 *  - Empfangen von RPC-Anfragen
 *
 * Benötigte Bibliotheken
 * ----------------------
 *
 * Im Library Manager der Arduino IDE müssen folgende Bibliotheken installiert werden.
 * Ohne sie kann die ThingsBoard-Klasse nicht kompiliert werden.
 *
 *  - PubSubClient
 *  - ArduinoJson
 */

#include <ArduinoJson.h>

#include "wifi-wizard.h"
#include "thingsboard.h"


constexpr char *tb_host  = "thingsboard.zimolong.eu";
constexpr int   tb_port  = 21;
constexpr char *tb_token = "OKFx46PoESgumH4jUavF";
ThingsBoard thingsboard  = {};

bool enabled = true;


void setup() {
  delay(1000);
  Serial.begin(115200);

  wifi_wizard();

  thingsboard.connect(tb_host, tb_port, tb_token);                  // Verbindung zum Server herstellen
  thingsboard.subscribe_attribute_values(on_attribute_values);      // Shared Attribute Werte vom Server empfangen
  thingsboard.send_attribute_request("enabled");                    // Aktuellen Wert des Shared Attribute "enabled" anfordern
}


void loop() {
  // Datenempfang sicherstellen
  thingsboard.loop();

  // Messwerte senden
  if (!enabled) return;

  static long prev_millis = 0;
  long current_millis = millis();

  if (current_millis - prev_millis >= 1000) {
    prev_millis = current_millis;

    JsonDocument values;
    values["hall"] = hallRead();                  // Internet Hall-Magnetfeld-Sensor des ESP32

    thingsboard.send_telemetry(values);
  }
}


void on_attribute_values(JsonObject values) {
  if (values.containsKey("enabled")) {            // Prüfen, ob der gewünschte Wert überhaupt gesendet wurde
     enabled = values["enabled"].as<bool>();      // Empfangenen Wert in eine globale Variable übernehmen
  }
}