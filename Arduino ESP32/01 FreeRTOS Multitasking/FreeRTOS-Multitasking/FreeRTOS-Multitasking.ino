/**
 * FreeRTOS Multitasking - Minimalbeispiel
 * =======================================
 *
 * Dieses Beispiel zeigt, wie mit der Funktion xTaskCreate() weitere Hintergrundtasks
 * gestartet werden können. Ein Task ist dabei einfach eine Funktion, die parallel zu
 * den anderen Tasks läuft. Dabei muss beachtet werden, dass Taskfunktion nie beendet
 * werden dürfen und deshalb eine Endlosschleife beinhalten müssen.
 *
 * Zusätzlich muss man beachten, dass FreeRTOS ein kooperatives Multitasking umsetzt.
 * Ein Task muss daher aktiv die Kontrolle an FreeRTOS zurückgeben, damit andere Tasks
 * eine Chance bekommen, zu laufen. Dies geschieht durch den regelmäßigen Aufruf einer
 * der folgenden Funktionen:
 *
 *   - taskYIELD(): Gibt die Kontrolle an FreeRTOS zurück
 *   - delay(): Pausiert den Task und lässt derweil andere Tasks laufen
 *   - vTaskDelay(): Unterbricht den Task für die gegebene Anzahl "Ticks"
 *   - vTaskDelayUntil(): Unterbricht den Task für eine gegene Zeitdauer
 *   - ...
 *
 * In diesem Beispiel starten wir zwei mal die Funktion myBackgroundTask(). Natürlich
 * müssen nicht alle Tasks dieselbe Funktion nutzen. Man kann aber, wie hier gezeigt,
 * mehrere Instanzen derselben Funktion als Tasks starten, wenn man will.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * xTaskCreate():  https://www.freertos.org/a00125.html
 * taskYIELD():    https://www.freertos.org/a00020.html#taskYIELD
 * vTaskGetInfo(): https://www.freertos.org/vTaskGetInfo.html
 */

#include "freertos/task.h"


// Parameter für die Hintergrundtasks
struct MyTaskParameters {
  int taskNumber;           // Tasknummer für Konsolenausgabe
  int initialDelay_ms;      // Verzögerung, damit die Tasks abwechselnd auf die Konsole zugreifen
};

struct MyTaskParameters taskParameters1 = {1, 1000};
struct MyTaskParameters taskParameters2 = {2, 2000};


// Konstanten
constexpr int delay_ms = 3000;


/**
 * Arduino Setup-Funktion
 */
void setup() {
  Serial.begin(115200);

  xTaskCreate(
    /* pcTaskCode    */  myBackgroundTask,             // Auszuführende Funktion
    /* pcName        */  "Hintergrund-Task 1",         // Name des Tasks
    /* uxStackDepth  */  2048,                         // Stack-Größe in Words
    /* pvParameters  */  (void*) &taskParameters1,     // Zeiger auf Task-Parameter oder NULL
    /* uxPriority    */  2,                            // Task-Priorität
    /* pxCreatedTask */  NULL                          // Zeiger auf TaskHandle_t oder NULL
  );

  xTaskCreate(
    /* pcTaskCode    */  myBackgroundTask,
    /* pcName        */  "Hintergrund-Task 2",
    /* uxStackDepth  */  2048,
    /* pvParameters  */  (void*) &taskParameters2,
    /* uxPriority    */  2,
    /* pxCreatedTask */  NULL
  );
}


/**
 * Arduino Loop-Funktion
 */
void loop() {
  Serial.println("Arduino-Haupttask");
  delay(delay_ms);
}


/**
 * Zusätzliche mit FreeRTOS gestartete Tasks. Bachten Sie, dass wir dieselbe
 * Funktion mehrfach starten und dabei jeweils andere Parameter übergeben.
 */
void myBackgroundTask(void* pvParameters) {
  // Typecast, um den Zeiger wieder auf den richtigen Typ zu ändern
  struct MyTaskParameters* parameters = (struct MyTaskParameters*) pvParameters;

  delay(parameters->initialDelay_ms);

  while (true) {
    Serial.printf("Hintergrundtask %i\n", parameters->taskNumber);
    delay(delay_ms);
  }
}
