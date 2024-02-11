/**
 * WiFi-Station
 * ============
 *
 * Dieses Beispiel zeigt, wie sich der ESP32 an einem WLAN anmelden und daraufhin Daten
 * von einem Server abrufen kann. Im Gegensatz zu den meisten Beispielen im Netz nutzen
 * wir heir nicht die Arduino WiFi Library, da diese nicht mit WPA2 Enterprise (brauchen
 * für das DHBW-WLAN) zurecht kommt und laut Arduino-Dokumentation nicht mehr gewartet wird.
 * Stattdessen nutzen wir die leider etwas umständlicheren WiFi-Funktionen des ESP32 SDK.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * xxx
 */

#include <esp_log.h>
#include <esp_event.h>
#include <esp_wifi.h>

static const char* TAG              = "WiFi-Beispiel";
static const char* MY_WIFI_SSID     = "Emil-Wachter-Str. 14";
static const char* MY_WIFI_PASSWORD = "DanaKatharinaZimolong";

void setup() {
  Serial.begin(115200);

  // WLAN-Initialisierung
  ESP_LOGI(TAG, "Initialisiere WiFi");

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
    /* event_base        */  WIFI_EVENT,
    /* event_id          */  ESP_EVENT_ANY_ID,
    /* event_handler     */  my_wifi_event_handler,
    /* event_handler_arg */  NULL,
    /* instance          */  NULL
  ));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
    /* event_base        */  IP_EVENT,
    /* event_id          */  ESP_EVENT_ANY_ID,
    /* event_handler     */  my_ip_event_handler,
    /* event_handler_arg */  NULL,
    /* instance          */  NULL
  ));

  wifi_config_t wifi_config = {};
  wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  wifi_config.sta.pmf_cfg.capable    = true;
  wifi_config.sta.pmf_cfg.required   = false;

  strncpy((char*) wifi_config.sta.ssid, MY_WIFI_SSID, 32);
  strncpy((char*) wifi_config.sta.password, MY_WIFI_PASSWORD, 64);

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "WiFi-Initialisierung abgeschlossen");
}


void my_wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  switch (event_id) {
    case WIFI_EVENT_STA_START:
      ESP_LOGI(TAG, "Stelle WiFI-Verbindung her");
      esp_wifi_connect();
      break;

    case WIFI_EVENT_STA_DISCONNECTED:
      ESP_LOGI(TAG, "WiFi-Verbindung getrennt, versuche wiederherzustellen");
      esp_wifi_connect();
      break;
  }
}


void my_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  switch (event_id) {
    case IP_EVENT_STA_GOT_IP: {
      ip_event_got_ip_t* event_data = (ip_event_got_ip_t*) event_data;
      ESP_LOGI(TAG, "Zugewiesene IP-Adresse:" IPSTR, IP2STR(&event_data->ip_info.ip));
      break;
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
}