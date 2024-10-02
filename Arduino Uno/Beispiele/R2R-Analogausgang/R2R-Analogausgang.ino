/**
 * Analogspannung mit R2R-Widerstandsleiter
 * ========================================
 *
 * Dieser Sketch zeigt, wie man mit einer Hand voll Widerstände eine einfache R2R-Widerstandsleiter
 * zum Erzeugen variabler Spannungen aufbauen kann. Dies ist eine Möglichkeit, die Limitierung des
 * Arudino Uno zu umgehen, keine analogen Ausgangspins zu besitzen. Der Vorteil dieser Lösung ist,
 * dass sie einfach und günstig zu realisieren ist. Der Nachteil ist, dass sie viele Pins belegt
 * (je Bit eines) und die Auflösung von der Anzahl der verwendeten Pins abhängt.
 *
 * Das Pin-Problem könnte man durch ein serielles Shiftregister (Serial In / Parallel Out) oder
 * einen GPIO-Extender-Baustein lösen. Dann kann man aber auch gleich einen D/A-Konverter als
 * Baustein anschließen.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Siehe Hardwareskizze und CircuitJS-Schaltplan.
 *
 * HINWEIS: Anstelle der Widerstände kann auch an jeden Ausgangspin eine LED angeschlossen werden,
 * um nachzuvollziehen, für welchen Wert welche Ausgänge geschaltet werden. Man sieht dann schön,
 * dass die einzelnen Ausgänge den einzelnen Bits einer Binärzahl entsprechen.
 *
 * Aufgaben
 * --------
 *
 * Bauen Sie die Schaltung nach und messen Sie mit einem Multimeter die verschiedenen Spannungen,
 * wenn Sie die beiden Buttons betätigen. Zusätlich können Sie die Spannungen mit einem Oszilloskop
 * sichtbar machen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
 */

constexpr int pin_btn_dec = 7;
constexpr int pin_btn_inc = 6;
constexpr int pins_dac[]  = {11, 12, 13};
constexpr int n_pins_dac  = sizeof(pins_dac) / sizeof(int);
constexpr int max_value   = pow(2, n_pins_dac) - 1;
constexpr int debounce_ms = 100;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(pin_btn_dec, INPUT_PULLUP);
  pinMode(pin_btn_inc, INPUT_PULLUP);

  for (int i = 0; i < n_pins_dac; i++) {
    pinMode(pins_dac[i], OUTPUT);
  }

  delay(1000);
  Serial.begin(9600);
  Serial.println("Bereit ...");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int value = 0;
  static int prev_value = -1;

  // Buttons abfragen
  if (digitalRead(pin_btn_dec) == LOW) {
    delay(debounce_ms);

    if (digitalRead(pin_btn_dec) == LOW) {
      if (value > 0) value--;
    }
  }

  if (digitalRead(pin_btn_inc) == LOW) {
    delay(debounce_ms);

    if (digitalRead(pin_btn_inc) == LOW) {
      if (value < max_value) value++;
    }
  }

  // Wert auf der seriellen Konsole ausgeben
  if (value != prev_value) {
    prev_value = value;

    Serial.print("Neuer Wert: ");
    Serial.println(value);
  }

  // Spannung erzeugen
  for (int i = 0; i < n_pins_dac; i++) {
    digitalWrite(pins_dac[i], (value >> i) & 0x01);
  }
}
