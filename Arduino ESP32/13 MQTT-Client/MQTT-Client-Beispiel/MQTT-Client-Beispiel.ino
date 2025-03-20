/**
 * MQTT Client
 * ===========
 *
 * Dieses Beispiel zeigt, wie sich der ESP32 mit einem MQTT Broker im Internet verbinden kann
 * um darüber Nachrichten mit anderen IoT-Devices oder einem IoT-Backend auszutauschen. In
 * dieser Version spricht der ESP32 einfach mit sich selbst, indem er dasselbe Topic abonniert,
 * an das er auch Nachrichten sendet. Das Beispiel kann aber leicht angepasst werden, um z.B.
 * zwei ESP32 miteinander kommunizieren zu lassen.
 *
 * Benötigte Bibliotheken
 * ----------------------
 *
 * Im Library Manager der Arduino IDE muss folgende Bibliothek installiert werden:
 *
 *  - PubSubClient
 *
 * Danach kann der Quellcode gebaut und hochgeladen werden. Das Programm fragt beim Start nach
 * den WLAN-Zugangsdaten in der seriellen Konsole, so dass diese nicht im Quellcode hinterlegt
 * werden müssen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.emqx.com/en/blog/esp32-connects-to-the-free-public-mqtt-broker
 */

#include <PubSubClient.h>
#include "wifi-wizard.h"

// Pin der eingebauten LED
constexpr int LED_BUILTIN = 2;


// MQTT-Client
struct {
  char *host     = "broker.emqx.io";
  int   port     = 1883;
  char *username = "emqx";
  char *password = "public";

  // Beispiel: Der ESP32 kann natürlich auch mehrere Topics ansprechen
  char* publish_topic   = "wahlmodul-iot/esp32";
  char* subscribe_topic = "wahlmodul-iot/#";
} mqtt_broker;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void on_mqtt_message_received(char *topic, byte *payload, unsigned int length);


void setup() {
  // WLAN-Verbindung herstellen
  Serial.begin(115200);
  delay(1000);

  wifi_wizard();

  // Verbindung zum MQTT-Broker herstellen
  mqtt_client.setServer(mqtt_broker.host, mqtt_broker.port);
  mqtt_client.setCallback(on_mqtt_message_received);

  String client_id = "wmiot-esp32-" + String(WiFi.macAddress());
  Serial.printf("Stelle Verbindung zum MQTT-Broker her: mqtt://%s:%i, CliendID: %s\n", mqtt_broker.host, mqtt_broker.port, client_id.c_str());

  while (!mqtt_client.connected()) {
    if (!mqtt_client.connect(client_id.c_str(), mqtt_broker.username, mqtt_broker.password)) {
      Serial.print(".");
      delay(1000);
    }
  }

  Serial.println("OK");

  mqtt_client.subscribe(mqtt_broker.subscribe_topic);
}


void loop() {
  // Wichtig, damit wir Nachrichten empfangen können!
  mqtt_client.loop();

  // Nachricht senden
  static unsigned long prev_millis = 0;
  unsigned long current_millis = millis();

  if (current_millis - prev_millis > 1000) {
    prev_millis = current_millis;

    Serial.println("Sende Nachricht!");
    mqtt_client.publish(mqtt_broker.publish_topic, "Der ESP32 sagt hallo!");
  }
}


void on_mqtt_message_received(char *topic, byte *payload, unsigned int length) {
  // Eingebaute LED blinken
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  // Nachricht auf der seriellen Konsole ausgeben
  Serial.println("Empfange Nachricht!");
  Serial.printf("Topic.....: %s\n", topic);
  Serial.print ("Payload...:");

  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }

  Serial.println();
  Serial.println();
}
