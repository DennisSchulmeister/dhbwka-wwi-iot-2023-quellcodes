/**
 * Zwei Buttons abfragen
 * =====================
 *
 * Musterlösung zur Aufgabe "Button mit Active High vs. Active Low". Dieses Programm fragt zwei an den Pins
 * 6 und 7 angeschlossenen Buttons ab und zeigt an, wenn sich ihr Wert ändert. Button 6 wird Active High
 * ausgelesen, muss daher bei Betätigung des Buttons 5V auf den Eingangspin legen. Button 7 wird Active Low
 * angesteuert und muss den Pin bei Betägigung daher nach GND durchschalten (um 5V aus dem Arduino abfließen
 * zu lassen).
 *
 * HINWEIS: Die Buttons werden hier absichtlich nicht entprellt. Sie werden daher feststellen, dass der
 * erkannte Wert ein paar mal hin und her schwankt, wenn Sie einen Button drücken.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Siehe Schaltpläne in den Folien.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/
 */

constexpr int button_active_high_pin = 6;
constexpr int button_active_low_pin  = 7;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(button_active_high_pin, INPUT);
  pinMode(button_active_low_pin,  INPUT_PULLUP);

  Serial.begin(9600);
  delay(1000);
  Serial.println("Bereit ...");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static bool button_active_high_value1 = LOW;
  static bool button_active_high_value2 = LOW;

  static bool button_active_low_value1  = HIGH;
  static bool button_active_low_value2  = HIGH;

  button_active_high_value2 = digitalRead(button_active_high_pin);
  button_active_low_value2  = digitalRead(button_active_low_pin);

  if (button_active_high_value1 != button_active_high_value2) {
    button_active_high_value1 = button_active_high_value2;

    Serial.print("Button an Pin ");
    Serial.print(button_active_high_pin);
    Serial.print(" hat neuen Wert ");
    Serial.print(button_active_high_value2);
    Serial.print(".\n");
  }

  if (button_active_low_value1 != button_active_low_value2) {
    button_active_low_value1 = button_active_low_value2;

    Serial.print("Button an Pin ");
    Serial.print(button_active_low_pin);
    Serial.print(" hat neuen Wert ");
    Serial.print(button_active_low_value2);
    Serial.print(".\n");
  }
}
