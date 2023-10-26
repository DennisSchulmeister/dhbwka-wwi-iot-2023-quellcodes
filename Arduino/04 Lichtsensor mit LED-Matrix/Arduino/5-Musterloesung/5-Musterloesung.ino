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

// Konstanten und Variablen für die einzelnen Modi des Devices
constexpr int MODE_STARTUP  = 0;
constexpr int MODE_NORMAL   = 1;
constexpr int MODE_LED_TEST = 2;

int mode = MODE_STARTUP;


/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  button.setup();

  for (int led_pin = LED_PIN_START; led_pin <= LED_PIN_END; led_pin++) {
    pinMode(led_pin, OUTPUT);
  }
}


/**
 * Unendlich oft ausgeführte Programmlogik
 */
void loop() {
  // Testmodus aktivieren, wenn der Knopf gedrückt wird
  if (mode != MODE_STARTUP && !button.digitalRead()) {
    mode = MODE_LED_TEST;
  }

  // Entsprechendes Unterprogramm je nach Betriebsmodus ausführen
  switch (mode) {
    case MODE_STARTUP:
      loop_startup();
      break;
    case MODE_NORMAL:
      loop_normal();
      break;
    case MODE_LED_TEST:
      loop_test();
      break;
  }

  // LED-Matrix ansteuern
  // Erfoglt hier AVR-spezifisch, um alle Pins eines Ports gleichzeitig setzen zu können.
  // Die LED-Matrix wird hierfür über den "Port D" des Atmega-Microcontrollers angesprochen:
  //
  // +----------+----------+----------+----------+---------+---------+-------+-------+
  // | Bit 7    | Bit 6    | Bit 5    | Bit 4    | Bit 3   | Bit 2   | Bit 1 | Bit 0 |
  // +----------+----------+----------+----------+---------+---------+-------+-------+
  // | Spalte 4 | Spalte 3 | Spalte 2 | Spalte 1 | Zeile 2 | Zeile 1 |       |       |
  // +----------+----------+----------+----------+---------+---------+-------+-------+
  //
  // Damit die LEDs nicht zu dunkel leuchten, wird nach dem Setzen einer Zeile kurz gewartet,
  // bevor die nächste LED-Zeile aktiviert wird.
  PORTD = 4 | (~led_matrix & 0x0F) << 4;
  delayMicroseconds(100);
  PORTD = 8 | (~led_matrix & 0xF0);
  delayMicroseconds(100);
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
  int sensor_value = 1023 - analogRead(SENSOR);
  
  if      (sensor_value >= 960) led_matrix = B11111111;
  else if (sensor_value >= 832) led_matrix = B01111111;
  else if (sensor_value >= 704) led_matrix = B00111111;
  else if (sensor_value >= 576) led_matrix = B00011111;
  else if (sensor_value >= 448) led_matrix = B00001111;
  else if (sensor_value >= 320) led_matrix = B00000111;
  else if (sensor_value >= 192) led_matrix = B00000011;
  else if (sensor_value >= 64)  led_matrix = B00000001;
  else                          led_matrix = B00000000;
}


/**
 * Testmodus: LEDs der Reihe nach ein- und wieder ausschalten
 */
void loop_test() {
  // Funktion nur alle halbe Sekunde ausführen
  static unsigned long previous_millis  = millis();
  unsigned long        current_millis   = millis();
  constexpr int        delay_millis     = 500;

  if ((current_millis - previous_millis) < delay_millis) return;
  previous_millis = current_millis;

  // LEDs der Reihe nach ein- und ausschalten
  constexpr int values[] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00
  };

  constexpr int max_index = sizeof(values) / sizeof(values[0]);
  static int index = -1;

  index++;

  if (index >= max_index) {
    index = 0;
    mode = MODE_NORMAL;
    return;
  }

  led_matrix = values[index];
}
