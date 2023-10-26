/**
 * Temparatursensor mit LED-Matrix
 * ===============================
 *
 * Praxisbeispiel für die Anwendung der Elektronikgrundlagen beim Aufbau einer einfachen
 * Microcontroller-Schaltung mit Arduino. Der Arduino misst eine von einem Sensor erzeugte
 * variable Spannung, die durch den inneren Aufbau des Sensors als Spannungsteiler entsteht.
 * Der gemessene Wert wird auf einer LED-Bar mit 8 LEDs angezeigt, wobei die LEDs über ein
 * Matrixschaltung angesprochen werden.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  Siehe beigefügte Breadboard-Skizze. Hier die Kurzversion:
 *
 *   * Pin A0: KY-018 Helligkeitssensor oder anderen Spannungsteiler
 *   * Pin D8: Button, der nach Masse/Ground schließt
 *   * Pins D2...D3: LED-Matrix Zeilen je mit 200 Ohm Vorwiderstand
 *   * Pins D4...D7: LED-Matrix Spalten
 *
 * Der Spannungsteiler muss am einen Ende gegen 5V und am anderen Ende gegen
 * Masse/Ground verbunden werden. Der mittlere Ausgang, an dem die variable
 * Spannung anliegt, wird mit A0 verbunden, um die Spannung zu messen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://sensorkit.joy-it.net/de/sensors/ky-018
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
 *
 * Zur Funktionsweise der LED-Matrixschaltung siehe die Kommentare im dazugehörigen
 * Beispielprogramm.
 */

#include "DebouncedDigitalInput.h"

// Konstanten und Variablen für die Sensoren
auto          button = DebouncedDigitalInput(8, true, 100);
constexpr int SENSOR = A0;

// Konstanten und Variablen für die LED-Matrix
constexpr int LED_PIN_START = 2;
constexpr int LED_PIN_END   = 7;

uint8_t led_matrix = 0xFF;


/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  button.setup();

  // TODO: Pins der LED-Matrix als Ausgangspins konfigurieren
}


/**
 * Unendlich oft ausgeführte Programmlogik
 */
void loop() {
  // TODO: Über eine globale Variable soll gesteuert werden, in welchem "Modus" sich das
  // Device befindet. Je nach Modus soll dann in eine andere Funktion abgesprungen werden,
  // welche die jeweilige Logik implementiert. Die Funktionen sind unten bereits definiert.


  // TODO: LED-Matrix ansteuern, wie im dazugehörigen Beispiel gezeigt
  
}


/**
 * Startmodus: LEDs nach einer halben Sekunde ausschalten und zu normalem Modus wechseln.
 */
void loop_startup() {
  static unsigned long previous_millis = millis();
  unsigned long        current_millis  = millis();
  constexpr int        delay_millis    = 500;

  led_matrix = 0xFF;

  if ((current_millis - previous_millis) < delay_millis) return;
  previous_millis = current_millis;

  led_matrix = 0x00;
  mode = MODE_NORMAL;
}


/**
 * Normaler Betriebsmodus: Aktuellen Sensorwert messen und LEDs entsprechend aktivieren.
 */
void loop_normal() {
  // TODO: Sensorwert auslesen

  // TODO: Sensorwert in acht gleichgroße Bereiche einteilen und für jeden Bereich das
  // jeweilige Bitmuster in die Variable led_matrix schreiben, um die LED-Bar aufleuchten
  // zu lassen: B00000000, B00000001, B00000011, B00000111, B00001111, … Entspricht dezimal
  // immer einer Zweierpotenz minus eins, also 1, 3, 7, 15, …
}


/**
 * Testmodus: LEDs der Reihe nach ein- und wieder ausschalten
 */
void loop_test() {
  // TODO: Wie bei der Funktion loop_startup() gezeigt, sicherstellen, dass die Logik dieser
  // Funktion nur alle halbe Sekunde ausgeführt wird. Die Funktion daher vorzeitig verlassen,
  // wenn seit dem letzten Durchlauf keine halbe Sekunde vergangen ist.

  // TODO: Beim ersten und letzten Durchlauf alle LEDs auschalten. Entsprechend dazwischen erst
  // die erste, später dann nur die zweite, dann die dritte, … LED einschalten. Nach der letzten
  // LED wieder rückwärts gehen und vorletzte, dann die vorvorletzte LED einschalten usw.
  // Hinweis: Die einfachste Lösung ist, ein Array mit den Binärwerten für die LED-Matrix zu
  // definieren und bei jedem Durchlauf einfach den nächsten Eintrag aus dem Array zu holen.

  // TODO: Vergessen Sie nicht, nach dem letzten Durchlauf die static-Variablen wieder zurückzusetzen,
  // damit die Funktion später erneut durchlaufen werden kann.
}
