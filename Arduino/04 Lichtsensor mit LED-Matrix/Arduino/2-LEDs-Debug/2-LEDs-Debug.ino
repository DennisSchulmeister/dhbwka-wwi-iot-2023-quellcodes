/**
 * Debugging der LED-Matrix
 * ========================
 *
 * Hilfsprogramm zum Debuggen des Hardwareaufbaus für die LED-Matrix. Verwenden Sie
 * dieses Programm während dem Aufbau der Schaltung, um zu prüfen, ob alle LEDs leuchten.
 * Wenn dies klappt, laden Sie das nächste Programm, um die LEDs einzeln zu testen.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  Siehe beigefügte Breadboard-Skizze. Hier die Kurzversion:
 *
 *   * Pins D2...D3: LED-Matrix Zeilen je mit 200 Ohm Vorwiderstand
 *   * Pins D4...D7: LED-Matrix Spalten
 */

constexpr int LED_PIN_ROW1 = 2;
constexpr int LED_PIN_ROW2 = 3;
constexpr int LED_PIN_COL1 = 4;
constexpr int LED_PIN_COL2 = 5;
constexpr int LED_PIN_COL3 = 6;
constexpr int LED_PIN_COL4 = 7;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(LED_PIN_ROW1, OUTPUT);
  digitalWrite(LED_PIN_ROW1, HIGH);

  pinMode(LED_PIN_ROW2, OUTPUT);
  digitalWrite(LED_PIN_ROW2, HIGH);

  pinMode(LED_PIN_COL1, OUTPUT);
  digitalWrite(LED_PIN_COL1, LOW);

  pinMode(LED_PIN_COL2, OUTPUT);
  digitalWrite(LED_PIN_COL2, LOW);

  pinMode(LED_PIN_COL3, OUTPUT);
  digitalWrite(LED_PIN_COL3, LOW);

  pinMode(LED_PIN_COL4, OUTPUT);
  digitalWrite(LED_PIN_COL4, LOW);
}

/**
 * Unendlich oft ausgeführte Programmlogik
 */
void loop() {
}

