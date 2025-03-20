/**
 * ThingsBoard IoT-Backend (Einstiegsbeispiel)
 * ===========================================
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

#include <esp_log.h>
#include <ArduinoJson.h>

#include "wifi-wizard.h"
#include "thingsboard.h"


constexpr char *tb_host  = "thingsboard.zimolong.eu";
constexpr int   tb_port  = 21;  //1883;
constexpr char *tb_token = "iHcxxkfVHJgbH2m5B13q";
ThingsBoard thingsboard  = {};

bool enabled = false;


void setup() {
  delay(1000);
  Serial.begin(115200);

  wifi_wizard();
  ESP_LOGI("WiFi", "OK! Wir sind verbunden ...");

  ESP_LOGI("ThingsBoard", "Stelle Verbindung zu ThingsBoard her.");
  thingsboard.connect(tb_host, tb_port, tb_token);                  // Verbindung zum Server herstellen
  thingsboard.subscribe_attribute_values(on_attribute_values);      // Shared Attribute Werte vom Server empfangen
  thingsboard.send_attribute_request("enabled");                    // Aktuellen Wert des Shared Attribute "enabled" anfordern
  thingsboard.subscribe_rpc_request(on_rpc_request);                // RPC-Anfrage vom Server verarbeiten
  ESP_LOGI("ThingsBoard", "Hat geklappt!");
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
    values["random"] = random(25,33);                  // Zufallszahl zwischen 25 und 33

    ESP_LOGI("ThingsBoard", "Sende Messwert %d", values["random"].as<long>());
    thingsboard.send_telemetry(values);
  }
}


void on_attribute_values(JsonObject values) {
  if (values.containsKey("enabled")) {            // Prüfen, ob der gewünschte Wert überhaupt gesendet wurde
    enabled = values["enabled"].as<bool>();      // Empfangenen Wert in eine globale Variable übernehmen
    ESP_LOGI("ThingsBoard", "Empfange Shared Attribut enabled mit Wert %d", enabled);
  }
}


void on_rpc_request(int request_id, const char *method, JsonObject parameters) {
  String _method = method;

  if (_method.equals("setValue")) {
    String attribute = parameters["attribute"];
    String value     = parameters["value"];

    ESP_LOGI("ThingsBoard", "Empfange RPC setValue %s mit Wert %s", parameters["attribute"], parameters["value"]);

    if (attribute.equals("enabled")) {
      enabled = value.equals("true");
    }
  }
}
