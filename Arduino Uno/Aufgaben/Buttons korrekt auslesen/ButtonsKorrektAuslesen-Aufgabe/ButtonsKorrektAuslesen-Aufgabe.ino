/**
 * Aufgabe: Buttons korrekt auslesen
 * =================================
 *
 * In dieser Aufgabe lernen Sie, Buttons so auszulesen, dass Sie auf einfache Weise entprellt werden
 * und auch nur einmal auslösen, während sie gedrückt werden. Das Entprellen ist wichtig, da der
 * mechanische Kontakt die Verbindung (hier gegen Ground) nicht sofort stabil herstellt, sondern
 * für eine kurze Weile "ungültige" Spannungen in der Mitte von 5V und 0V produziert. Das einmalige
 * Auslösen ist darüber hinaus wichtig, damit man den Button beliebig lange gedrückt halten kann,
 * ohne die damit verknüpfte Aktion mehrfach auszuführen.
 *
 * Bauen Sie hierfür die Hardwareschaltung nach und testen sie mit diesem Programm. Passen Sie das
 * Programm anschließend so an, dass die Buttons entprellt werden nur einmal auslösen, wenn sie gedrückt
 * werden. Als Vorlage hierfür können Sie das Beispielprogramm "Buttons entprellen" verwenden.2
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  * GND -> Button -> Pin 8
 *  * GND -> Button -> Pin 9
 *  * GND -> Button -> Pin 10
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/
 * https://docs.arduino.cc/language-reference/en/functions/time/delay/
 */

constexpr int button1_pin = 8;
constexpr int button2_pin = 9;
constexpr int button3_pin = 10;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Bereit ...");

  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  if (digitalRead(button1_pin) == LOW) {
    Serial.println("Button 1");
  } else if (digitalRead(button2_pin) == LOW) {
    Serial.println("Button 2");
  } else if (digitalRead(button3_pin) == LOW) {
    Serial.println("Button 3");
  }
}
