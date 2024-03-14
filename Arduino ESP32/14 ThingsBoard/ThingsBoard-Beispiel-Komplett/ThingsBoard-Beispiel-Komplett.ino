/**
 * ThingsBoard IoT-Backend (Vollständiges Beispiel)
 * ================================================
 *
 * Dies ist ein vollständiges Ende-zu-Ende-Beispiel für einen IoT-Anwendungsfall mit dem ESP32
 * und dem ThingsBoard IoT-Backend. Der ESP32 misst hierfür über einen angeschlossenen BMP280-
 * Umweltsensor periodisch verschiedene Umweltdaten und sendet diese an das Backend. Im Backend
 * werden die Daten gespeichert, visualisiert und ausgewertet. Liegen die Werte außerhalb des
 * erwünschten Bereichs, wird ein Alarm ansgelöst und an den ESP32 geschickt, damit dort ein
 * Warnsignal gesteuert werden kann.
 *
 * Hardwareaufbau
 * --------------
 *
 *   - BMP280 Umweltsensor:
 *      - GPIO21 -> SDA
 *      - GPIO22 -> SCL
 *      - 3V3    -> SDO, CSB, VCC
 *      - GND    -> GND
 *
 *   - Lautsprecher für die Sprachausgabe:
 *      - GPIO25 (DAC_1) -> Left Audio (Signal Lautsprecher 1)
 *      - GPIO26 (DAC_2) -> Right Audio (Signal Lautsprecher 2)
 *      - GND -> Masse
 *
 * Benötigte Bibliotheken
 * ----------------------
 *
 * Im Library Manager der Arduino IDE müssen folgende Bibliotheken installiert werden.
 *
 *  - PubSubClient
 *  - ArduinoJson
 *  - Adafruit_BMP280 (mit Abhängigkeiten)
 *
 * Bibliothek ESP32-audioI2S
 * -------------------------
 *
 * Für das Alarmsignal wird, anstelle eines gewöhnlichen Buzzers, die Audio-Bibliothek
 * ESP32-audioI2S für einen gesprochenen Hinweis verwendet. Die Bibliothek muss unter
 * folgender Adresse heruntergeladen werden: https://github.com/schreibfaul1/ESP32-audioI2S/tags
 *
 * Zur Installation laden Sie eine ZIP-Datei herunter und speichern diese ohne Entpacken
 * ab. Anschließend klicken Sie in der Arduino IDE auf "Sketch -> Include Library ->
 * Add .ZIP Library" und fügen die ZIP-Datei dort dem Projekt hinzu.
 *
 * Lizenz
 * ------
 *
 * Die verwendete Bibliothek ESP32-audioI2S ist unter "GPL 3.0" lizenziert. Dies erfordert, dass
 * alle sie nutztenden Programme unter dieselbe oder eine neuere GPL-Lizenz fallen. Abweichend
 * zu den restlichen Beispielen ist dieser Quellcode daher unter der "GPL 3.0" oder nach Ihrer
 * Wahl einer neueren Version der GPL lizenziert.
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include <ArduinoJson.h>
#include <Audio.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "wifi-wizard.h"
#include "thingsboard.h"

#define TAG "main"

Adafruit_BMP280 bmp;

constexpr char *tb_host  = "thingsboard.zimolong.eu";
constexpr int   tb_port  = 1883;
constexpr char *tb_token = "nrcWIq669ShjmLKQtW4Y";
ThingsBoard thingsboard  = {};

Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);

bool measurement_enabled = true;
bool warning_signal = false;


void setup() {
  delay(1000);
  Serial.begin(115200);

  // BMP280-Sensor initialisieren
  ESP_LOGI(TAG, "Initialisiere BMP280-Umweltsensor");

  while (!bmp.begin()) {
    ESP_LOGE(TAG, "Fehler beim Initialisieren des BMP280-Umweltsensors. Bitte Verdrahtung prüfen.");
    delay(3000);
  }

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,     /* Betriebsart            */
    Adafruit_BMP280::SAMPLING_X2,     /* Temeratur Oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Luftdruck Oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filter                 */
    Adafruit_BMP280::STANDBY_MS_500   /* Standby-Zeit           */
  ); 

  // WLAN starten
  ESP_LOGI(TAG, "Starte WLAN-Konfiguration");

  wifi_wizard();

  // Verbindung zu ThingsBoard herstellen
  ESP_LOGI(TAG, "Stelle Verbindung zu ThingsBoard her");

  thingsboard.connect(tb_host, tb_port, tb_token);                  // Verbindung zum Server herstellen
  thingsboard.subscribe_attribute_values(on_attribute_values);      // Shared Attribute Werte vom Server empfangen
  thingsboard.send_attribute_request("measurement_enabled");        // Aktuellen Wert des Shared Attribute "measurement_enabled" anfordern
  thingsboard.send_attribute_request("warning_signal");             // Aktuellen Wert des Shared Attribute "warning_sigal" anfordern
  thingsboard.subscribe_rpc_request(on_rpc_request);                // RPC-Anfrage vom Server verarbeiten

  // Hintergrund-Task zum Ermitteln und Senden der Messwerte starten
  ESP_LOGI(TAG, "Starte Hintergrund-Task zum Ermitteln und Senden der Messwerte");

  xTaskCreate(
    /* pcTaskCode    */  measurement_task,
    /* pcName        */  "Messwerte ermitteln und senden",
    /* uxStackDepth  */  4096,
    /* pvParameters  */  NULL,
    /* uxPriority    */  2,
    /* pxCreatedTask */  NULL
  );

  // Hintergrund-Task für das Warnsignal starten
  ESP_LOGI(TAG, "Starte Hintergrund-Task für das Warnsignal");

  xTaskCreate(
    /* pcTaskCode    */  warning_task,
    /* pcName        */  "Warnsignal ausgeben",
    /* uxStackDepth  */  4096,
    /* pvParameters  */  NULL,
    /* uxPriority    */  2,
    /* pxCreatedTask */  NULL
  );

  // Kleine Statusmeldung ausgeben
  audio.setVolume(21);  // Maximum: 21
  audio.connecttospeech("Alles klar. Es kann los gehen!", "de");
}


void loop() {
  thingsboard.loop();
  audio.loop();
}


void on_attribute_values(JsonObject values) {
  String values_s;
  serializeJson(values, values_s);
  ESP_LOGI(TAG, "On Attribute Values: %s", values_s.c_str());

  if (values.containsKey("measurement_enabled")) {
      measurement_enabled = values["measurement_enabled"].as<bool>();
  }

  if (values.containsKey("warning_signal")) {
      warning_signal = values["warning_signal"].as<bool>();
  }
}


// Workaround für fehlende Funktion im ThingsBoard-Backend: Dort kann ein Toggle-Switch auf
// dem Dashboard zwar den Wert eines Attributs anzeigen, diesen aber nicht ändern. Es kann
// aber eine RPC-Anfrage an das Device geschickt werden, so dass das Device den Server
// hier auffordern kann, das Attribut zu ändern.
//
// ACHTUNG: Hierfür muss die Root Rule Chain in ThingsBoard angepasst werden, damit die
// zurückgesendeten Attribute als Shared Attributes gespeichert werden.
void on_rpc_request(int request_id, const char *method, JsonObject parameters) {
  String _method = method;

  String parameters_s;
  serializeJson(parameters, parameters_s);
  ESP_LOGI(TAG, "On RPC Request: %s, Parameter: %s", _method.c_str(), parameters_s.c_str());

  if (_method.equals("setAttributes")) {
    JsonDocument values;
    values.set(parameters);

    thingsboard.send_attributes(values);
  }
}


void measurement_task(void *pvParameters) {
  while (true) {
    delay(1000);
    if (!measurement_enabled) continue;

    JsonDocument values;
    values["hall"]        = hallRead();
    values["temperature"] = bmp.readTemperature();       // °C
    values["pressure"]    = bmp.readPressure() / 100;    // hPa
    values["altitude"]    = bmp.readAltitude(1013.25);   // m, Wert muss an den Standort angepasst werden

    String values_s;
    serializeJson(values, values_s);
    ESP_LOGI(TAG, "Aktuelle Messwerte: %s", values_s.c_str());

    thingsboard.send_telemetry(values);
  }  
}


void warning_task(void *pvParameters) {
  while (true) {
    delay(1000);
    if (!warning_signal) continue;

    ESP_LOGW(TAG, "Achtung, Lebensgefahr. Alle sofort dem Raum verlassen!");
    audio.connecttospeech("Achtung, Lebensgefahr. Alle sofort dem Raum verlassen!", "de");
    delay(4000);
  }
}