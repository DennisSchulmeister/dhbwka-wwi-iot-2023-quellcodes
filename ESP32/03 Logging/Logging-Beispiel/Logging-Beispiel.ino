/**
 * ESP32 Logging-Modul
 * ===================
 *
 * Dieses Beispiel zeigt, wie die Log-Makros des ESP32 SDK genutzt werden können,
 * um die Ausgabe von Log-Meldungen über die serielle Konsole zu vereinfachen.
 *
 * ACHTUNG: Vor dem Kompilieren muss über das Menü "Tools --> Core Debug Level"
 * ein Log-Level ausgewählt werden. Es werden dann nur Meldungen mit mindestens
 * der ausgewählten Priorität auf der seriellen Konsole ausgegeben. Standardmäßig
 * steht der Wert auf "None" (keine Logausgaben).
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/system/log.html
 */

#include <esp_log.h>


// Optional, aber empfohlen: Konstante für das Tag
static const char* TAG = "Mein Modul";

void setup() {
}

void loop() {
  int i = 0;

  // Makros zum Ausgeben von Log-Meldungen. Der letzte Buchstabe kennzeichnet
  // den Log-Level:
  //
  //  * ESP_LOGE: Error
  //  * ESP_LOGW: Warnung
  //  * ESP_LOGI: Information
  //  * ESP_LOGD: Debug
  //  * ESP_LOGV: Verbose
  //
  // Der erste Parameter ist das "Tag", mit dem normalerweise das Modul oder die
  // Anwendungsfunktion gekennzeichnet wird, welche die Meldung erzeugt.
  ESP_LOGI(TAG, "Dies ist eine Information (%i)", ++i);
  delay(2000);
}
