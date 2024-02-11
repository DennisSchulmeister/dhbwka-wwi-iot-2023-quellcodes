/**
 * FreeRTOS Multicore
 * ==================
 *
 * Dieses Beispiel zeigt aufbauend auf dem vorherigen Beispiel, wie die Tasks auf
 * unterschiedlichen CPU-Cores ausgeführt werden können. Denn standardmäßig laufen
 * alle Tasks auf dem selben Kern, auf dem auch der Arduino Haupttask läuft.
 *
 * Anstelle der Funktion xTaskCreate() wird einfach xTaskCreatePinnedToCore()
 * verwendet, die als letzten Parameter noch die Nummer des CPU-Kerns erwartet.
 * Diese kann beim ESP32 0, 1 oder tskNO_AFFINITY sein und muss ganz grundsätzlich
 * zwischen null und portNUM_PROCESSORS - 1 liegen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * xTaskCreate():             https://www.freertos.org/a00125.html
 * xTaskCreatePinnedToCore(): https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/system/freertos.html
 */

#include "freertos/task.h"


/**
 * Arduino Setup-Funktion
 */
void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    /* pcTaskCode    */  myTask1,                      // Auszuführende Funktion
    /* pcName        */  "My Task 1",                  // Name des Tasks
    /* uxStackDepth  */  2048,                         // Stack-Größe in Words
    /* pvParameters  */  NULL,                         // Zeiger auf Task-Parameter oder NULL
    /* uxPriority    */  2,                            // Task-Priorität
    /* pxCreatedTask */  NULL,                         // Zeiger auf TaskHandle_t oder NULL
    /* xCoreId       */  0                             // NEU: CPU-Core
  );

  xTaskCreatePinnedToCore(
    /* pcTaskCode    */  myTask2,
    /* pcName        */  "My Task 2",
    /* uxStackDepth  */  2048,
    /* pvParameters  */  NULL,
    /* uxPriority    */  2,
    /* pxCreatedTask */  NULL,
    /* xCoreId       */  1
  );
}


/**
 * Arduino Loop-Funktion
 */
void loop() {
  int core_id = xPortGetCoreID();

  Serial.printf("[loop]: CPU-Core %i\n", core_id);
  delay(3000);
}


/**
 * Task 1
 */
void myTask1(void* pvParameters) {
  int core_id = xPortGetCoreID();

  delay(1000);

  while (true) {
    Serial.printf("[myTask1]: CPU-Core %i\n", core_id);
    delay(3000);
  }
}


/**
 * Task 2
 */
void myTask2(void* pvParmaters) {
  int core_id = xPortGetCoreID();

  delay(2000);

  while (true) {
    Serial.printf("[myTask2]: CPU-Core %i\n", core_id);
    delay(3000);
  }
}
