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
    /* pcName        */  "Task auf Core 0",            // Name des Tasks
    /* uxStackDepth  */  2048,                         // Stack-Größe in Words
    /* pvParameters  */  NULL,                         // Zeiger auf Task-Parameter oder NULL
    /* uxPriority    */  2,                            // Task-Priorität
    /* pxCreatedTask */  NULL,                         // Zeiger auf TaskHandle_t oder NULL
    /* xCoreId       */  0                             // NEU: CPU-Core
  );

  xTaskCreatePinnedToCore(
    /* pcTaskCode    */  myTask2,
    /* pcName        */  "Task auf Core 1",
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
  Serial.println("[loop]: CPU-Core 0");
  delay(3000);
}


/**
 * Task auf CPU-Core 0
 */
void myTask1(void* pvParameters) {
  delay(1000);

  while (true) {
    Serial.println("[myTask1]: CPU-Core 0");
    delay(3000);
  }
}


/**
 * Task auf CPU-Core 1
 */
void myTask2(void* pvParmaters) {
  delay(2000);

  while (true) {
    Serial.println("[myTask2]: CPU-Core 1");
    delay(3000);
  }
}
