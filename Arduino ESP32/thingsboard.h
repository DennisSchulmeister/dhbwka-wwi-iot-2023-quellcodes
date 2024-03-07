/**
 * ThingsBoard Client
 * ==================
 *
 * Diese Header-Datei stellt einen einfache Klasse zur Verbindung mit einem ThingsBoard-Server
 * via MQTT zur Verfügung. Sie unterstützt dabei folgende Funktionen:
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
 * Im Library Manager der Arduino IDE müssen folgende Bibliotheken installiert werden:
 *
 *  - PubSubClient
 *  - ArduinoJson
 */

#pragma once

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <esp_log.h>
#include <cstring>

static const char* TAG = "ThingsBoard";
static const size_t BUFFER_SIZE = 128;

typedef void (*on_attribute_values_t)(JsonObject values);
typedef void (*on_rpc_request_t)(int request_id, const char *method, JsonObject parameters);
typedef void (*on_rpc_response_t)(int request_id, JsonDocument values);

/**
 * ThingsBoard Client via MQTT
 */
class ThingsBoard {
  public:

    /**
     * Konstruktor. Alle Parameter optional.
     */
    ThingsBoard(const char *client_id=0)
      : m_client_id(client_id),
        m_wifi_client(),
        m_mqtt_client(m_wifi_client) {

      this->m_mqtt_client.setSocketTimeout(10000);

      this->m_mqtt_client.setCallback([this](char *topic, byte *payload, unsigned int length) {
        this->on_mqtt_message_received(topic, payload, length);
      });

      if (!client_id) {
        this->m_client_id = WiFi.macAddress().c_str();
      }

      ESP_LOGI(TAG, "Verwende Client ID: %s", this->m_client_id);
    }

    /**
     * Verbindung zum Server herstellen. Parameter sind optional, wenn sie bereits
     * im Konstruktor mit Werten versorgt wurden.
     */
    void connect(const char *host, const int port, const char *access_token) {
      ESP_LOGI(TAG, "Verbindung herstellen zu Server mqtt://%s:%i", host, port);

      this->m_mqtt_client.setServer(host, port);

      while (!this->m_mqtt_client.connected()) {
        if (!this->m_mqtt_client.connect(this->m_client_id, access_token, "")) {
          switch (this->m_mqtt_client.state()) {
            case MQTT_CONNECTION_TIMEOUT:
              ESP_LOGI(TAG, "MQTT_CONNECTION_TIMEOUT");
              break;
            case MQTT_CONNECTION_LOST:
              ESP_LOGI(TAG, "MQTT_CONNECTION_LOST");
              break;
            case MQTT_CONNECT_FAILED:
              ESP_LOGI(TAG, "MQTT_CONNECT_FAILED");
              break;
            case MQTT_DISCONNECTED:
              ESP_LOGI(TAG, "MQTT_DISCONNECTED");
              break;
            case MQTT_CONNECTED:
              ESP_LOGI(TAG, "MQTT_CONNECTED");
              break;
            case MQTT_CONNECT_BAD_PROTOCOL:
              ESP_LOGI(TAG, "MQTT_CONNECT_BAD_PROTOCOL");
              break;
            case MQTT_CONNECT_BAD_CLIENT_ID:
              ESP_LOGI(TAG, "MQTT_CONNECT_BAD_CLIENT_ID");
              break;
            case MQTT_CONNECT_UNAVAILABLE:
              ESP_LOGI(TAG, "MQTT_CONNECT_UNAVAILABLE");
              break;
            case MQTT_CONNECT_BAD_CREDENTIALS:
              ESP_LOGI(TAG, "MQTT_CONNECT_BAD_CREDENTIALS");
              break;
            case MQTT_CONNECT_UNAUTHORIZED:
              ESP_LOGI(TAG, "MQTT_CONNECT_UNAUTHORIZED");
              break;
            default:
              ESP_LOGI("TAG", "Status: %i", this->m_mqtt_client.state());
          }

          delay(1000);
        }
      }

      this->m_mqtt_client.subscribe("v1/devices/me/attributes");
      this->m_mqtt_client.subscribe("v1/devices/me/attributes/response/+");
      this->m_mqtt_client.subscribe("v1/devices/me/rpc/request/+");
      this->m_mqtt_client.subscribe("v1/devices/me/rpc/response/+");
    }

    /**
     * Verbindung zum Server trennen.
     */
    void disconnect() {
      ESP_LOGI(TAG, "Verbindung mit Server trennen");
      this->m_mqtt_client.disconnect();
    }

    /**
     * In der Arduino loop()-Funktion aufzurufende Methode.
     */
    void loop() {
      this->m_mqtt_client.loop();
    }

    /**
     * Telemetrie-Daten an ThingsBoard senden.
     */
    void send_telemetry(JsonDocument values) {
      char payload[BUFFER_SIZE];
      serializeJson(values, payload);

      this->mqtt_publish("v1/devices/me/telemetry", payload);
    }

    void send_attributes(JsonDocument values) {
      char payload[BUFFER_SIZE];
      serializeJson(values, payload);

      this->mqtt_publish("v1/devices/me/attributes", payload);
    }

    /**
     * RPC-Anfrage an den Server schicken. Die Request ID dient der Zuordnung der Antwort
     * zur Anfrage, falls eine Antwort kommt.
     */
    void send_rpc_request(int request_id, const char *method, JsonDocument parameters = {}) {
      char payload[BUFFER_SIZE];

      JsonDocument message;
      message["method"] = method;
      message["params"] = parameters;
      
      serializeJson(message, payload);

      this->mqtt_publish("v1/devices/me/attributes", payload);
    }

    /**
     * Aktuellen Wert von einem oder mehrerer Shared Attributes beim Server anfragen.
     * Mehrere Attribute können durch Komma getrennt angegeben werden. Um die Werte
     * zu empfangen, muss zuvor mit `subscribe_attribute_values()` eine Callback-Funktion
     * registriert worden sein.
     */
    void send_attribute_request(const char *attributes) {
      char payload[BUFFER_SIZE];

      JsonDocument message;
      message["sharedKeys"] = attributes;

      serializeJson(message, payload);

      this->mqtt_publish("v1/devices/me/attributes/request/1", payload);
    }

    /**
     * Callback-Funktion für Attribut-Werte vom Server setzen.
     * ACHTUNG: Es kann nur eine Callback-Funktion geben.
     */
    void subscribe_attribute_values(on_attribute_values_t callback) {
      this->m_on_attribute_values = callback;
    }

    /**
     * Callback-Funktion für RPC-Anfragen vom Server setzen.
     * ACHTUNG: Es kann nur eine Callback-Funktion geben.
     */
    void subscribe_rpc_request(on_rpc_request_t callback) {
      this->m_on_rpc_request = callback;
    }

    /**
     * Callback-Funktion für RPC-Antworten vom Server setzen.
     * ACHTUNG: Es kann nur eine Callback-Funktion geben.
     */
    void subscribe_rpc_response(on_rpc_response_t callback) {
      this->m_on_rpc_response = callback;
    }
    
  private:

    /**
     * MQTT-Nachricht senden mit Protokollierung.
     */
    void mqtt_publish(const char *topic, const char *payload) {
      ESP_LOGD(TAG, "Sende Nachricht an Topic %s: %s", topic, payload);
      this->m_mqtt_client.publish(topic, payload);
    }

    /**
     * Callback-Funktion für empfangene MQTT-Nachrichten.
     */
    void on_mqtt_message_received(char *topic, byte *payload, unsigned int length) {
      char _payload[BUFFER_SIZE];
      strncpy(_payload, (char *) payload, length);

      String _topic = topic;

      ESP_LOGD(TAG, "Empfange Nachricht an Topic %s: %s", topic, _payload);

      JsonDocument values;
      if (payload) deserializeJson(values, _payload);

      if (_topic.equals("v1/devices/me/attributes") && this->m_on_attribute_values) {
        // Neue Attribut-Werte
        this->m_on_attribute_values(values.as<JsonObject>());
      } else if (_topic.startsWith("v1/devices/me/attributes/response/") && this->m_on_attribute_values) {
        // Angeforderte Attribute
        this->m_on_attribute_values(values["shared"]);
      } else if (_topic.startsWith("v1/devices/me/rpc/request/") && this->m_on_rpc_request) {
        // RPC-Anfrage vom Server
        int request_id     = atoi(_topic.substring(26).c_str());
        this->m_on_rpc_request(request_id, values["method"], values["params"]);
      } else if (_topic.startsWith("v1/devices/me/rpc/response/") && this->m_on_rpc_response) {
        // RPC-Antwort vom Server
        int request_id = atoi(_topic.substring(27).c_str());
        this->m_on_rpc_response(request_id, values);
      }
    }

    const char  *m_client_id;
    WiFiClient   m_wifi_client;
    PubSubClient m_mqtt_client;

    on_attribute_values_t m_on_attribute_values = 0;
    on_rpc_request_t      m_on_rpc_request      = 0;
    on_rpc_response_t     m_on_rpc_response     = 0;
};