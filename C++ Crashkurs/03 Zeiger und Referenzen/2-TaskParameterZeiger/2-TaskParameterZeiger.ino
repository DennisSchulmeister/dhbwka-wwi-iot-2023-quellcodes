/**
 * Zeiger für Task-Parameter
 * =========================
 *
 * Hier sehen wir ein praxisrelevantes Beispiel für die Verwendung von Zeigern auf dem ESP32
 * bzw. bei der Programmierung mit FreeRTOS. FreeRTOS kann mehrere Funktionen quasi-parallel
 * als Tasks ausführen. Da man auch dieselbe Funktion mehrfach parallel ausführen kann, lassen
 * sich dieser Parameter übergben, um die einzelnen Instanzen zu unterscheiden. Die Parameter
 * müssen hierfür als Zeiger (genauer gesagt Void-Pointer) übergeben werden, da dies die einzige
 * Möglichkeit in C/C++ ist, wie FreeRTOS die Übergabe beliebiger Werte ohne Typprüfung beim
 * Kompilieren zulassen kann.
 */

// Benötigt für die Funktion xTaskCreate()
#include "freertos/task.h"


// Strukturdefinition der an einen Task zu übergebenen Parameter
typedef struct {
  int         pre_delay;    // Wartezeit, bevor der Task losläuft
  const char* name;         // String zur Unterscheidung der Tasks
} my_task_parameters_t;

void setup() {
  Serial.begin(115200);

  my_task_parameters_t* task_parameters_1 = new my_task_parameters_t {0, "Task 1"};
  my_task_parameters_t* task_parameters_2 = new my_task_parameters_t {1000, "Task 2"};

  xTaskCreate(
    /* pcTaskCode    */  myTask,                       // Auszuführende Funktion
    /* pcName        */  NULL,                         // Name des Tasks
    /* uxStackDepth  */  2048,                         // Stack-Größe in Words
    /* pvParameters  */  task_parameters_1,            // Zeiger auf Task-Parameter oder NULL
    /* uxPriority    */  2,                            // Task-Priorität
    /* pxCreatedTask */  NULL                          // Zeiger auf TaskHandle_t oder NULL
  );

  xTaskCreate(
    /* pcTaskCode    */  myTask,                       // Auszuführende Funktion
    /* pcName        */  NULL,                         // Name des Tasks
    /* uxStackDepth  */  2048,                         // Stack-Größe in Words
    /* pvParameters  */  task_parameters_2,            // Zeiger auf Task-Parameter oder NULL
    /* uxPriority    */  2,                            // Task-Priorität
    /* pxCreatedTask */  NULL                          // Zeiger auf TaskHandle_t oder NULL
  );
}

void loop() {
}


/**
 * Als Task mehrfach parallel ausgeführte Funktion. Die Parameter werden als Zeiger übergeben.
 */
void myTask(void* pvParameters) {
  // 1: Type-cast, weil der Zeiger den Typ void hat (Klammerausdruck)
  // 2: Dereferenzieren des Zeigers, um an die Struktur zu kommen (Sternchen)
  // 3: Kopie der übergebenen Struktur in der Variable parameters speichern
  //    Warum eine Kopie= Weil der Variablentyp keine Referenz und kein Zeiger ist!
  //
  //                   (3)          (2)(1) 
  my_task_parameters_t parameters = *(my_task_parameters_t*) pvParameters;

  delay(parameters.pre_delay);

  while (true) {
    Serial.printf("Task: %s\n", parameters.name);
    delay(2000);
  }
}
