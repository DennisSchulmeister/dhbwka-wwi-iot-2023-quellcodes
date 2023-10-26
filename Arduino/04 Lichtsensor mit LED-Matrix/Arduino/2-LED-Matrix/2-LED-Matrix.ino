/**
 * Einfache 6-Bit LED-Matrix
 * =========================
 *
 * Minimalbeispiel zum Ansteuerng einer einfachen LED-Matrix mit 8 LEDS, die
 * über 6 Pins gesteuert werden. Das Prinzip skaliert sehr gut, da mit jedem
 * weiteren Pin die doppelte Anzahl an LEDs gesteuert werden kann.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  Siehe beigefügte Breadboard-Skizze. Hier die Kurzversion:
 *
 *   * Pins D2...D3: LED-Matrix Zeilen je mit 200 Ohm Vorwiderstand
 *   * Pins D4...D7: LED-Matrix Spalten
 *
 * Besonderheiten
 * --------------
 *
 * Die Matrix funktioniert generell so, dass jede LED mit zwei Ausgängen des
 * Arduino verbunden ist, die Zeile und Spalte genannt werden. Damit die LED
 * leuchtet, muss der Zeilenausgang HIGH und der Spaltenausgang LOW sein, so
 * dass Strom durch die LED fließen kann.
 *
 * In seiner Hauptschleife schaltet der Arduino einen Zeilenausgang HIGH und
 * die anderen LOW. Gleichzeitig aktiviert er für die aktuelle Zeile alle
 * Spaltenausgänge bis auf die, deren LED leuchten soll. Leider funktioniert
 * das mit der Arduino-Funktion digitalWrite() nicht gut, da diese immer nur
 * einen Ausgangspin schalten kann. Dadurch sind die anderen Ausgänge noch
 * in ihrem Zustand davor und die LEDs leuchten wild durcheinander.
 * 
 * Deshalb wird die Arduino-Funktion hier nicht genutzt. Stattdessen werden
 * die Ausgänge per Zugriff auf das Hardwareregister PORTB des Atmega zeitgleich
 * geschaltet. Leider ist der Code damit nicht mehr ohne Änderungen auf andere
 * Microcontroller portierbar. Aber er funktioniert und ist sehr kompakt.
 */

constexpr int LED_PIN_START = 2;
constexpr int LED_PIN_END   = 7;

uint8_t led_matrix = 0x00;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  for (int led_pin = LED_PIN_START; led_pin <= LED_PIN_END; led_pin++) {
    pinMode(led_pin, OUTPUT);
  }
}

/**
 * Unendlich oft ausgeführte Programmlogik
 *
 * HINWEIS: static-Variablen sind wie globale Variablen, die aber nur innerhalb
 * der Funktion angesprochen werden können.
 *
 * HINWEIS: % Modulo; << Shift Links; | Bit-OR; & Bit-AND; ~ Bit-NOT
 */
void loop() {
  // Jede halbe Sekunde eine andere LED aktivieren
  static int           led = -1;
  static unsigned long previous_millis = millis();
  unsigned long        current_millis  = millis();
  constexpr int        delay_millis    = 500;

  if ((current_millis - previous_millis) >= delay_millis) {
    prev_millis = current_millis;
    led = ++led % 8;
    led_matrix = 1 << led;
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
