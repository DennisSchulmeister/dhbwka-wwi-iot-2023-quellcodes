/**
 * Buttons entprellen
 * ==================
 *
 * Dieses Beispiel zeigt einen pragmatischen Ansatz, um an den Arduino angeschlossene Hardware-Buttons
 * zu entprellen. Sobald ein Pin auf LOW geht und der Button potentiell gedrückt wurde, wartet der
 * Arduino 100 Milisekunden (was auch bei schlechten Buttons noch funktionieren sollte) und prüft den
 * Pin dann nochmal. Ist dieser immer noch LOW, gehen wir davon aus, dass das Prellen aufgehört hat
 * und der Button immer noch gedrückt wird.
 *
 * Vorteil dieser Methode ist, dass sie simpel zu implementieren ist. Nachteil ist, dass der Arduino
 * während dieser 100 Milisekunden keine anderen Aufgaben bearbeiten kann. Hierfür müsste man sich
 * eine komplexere Logik ohne die delay()-Funktion überlegen, die bei jedem Aufruf der loop()-Funktion
 * eine Aufgabe ausführt und dabei im Falle von den Buttons mitzählt, wie viel Zeit vergangen ist.
 * Das Quellcode-Beispiel "Arduino Uno/Aufgaben/Hardware-Button" zeigt einen Weg, wie dies mit Hilfe
 * einer Klasse elegant realisiert werden kann. Auf dem ESP32 können wir stattdessen auch einfach
 * einen neuen Task starten und dadurch mehrer Aktionen quasi-parallel ausführen.
 *
 * Zusätzlich zeigt dieses Beispiel, wie ein Doppelauslösen der Buttons vermieden werden kann, wenn
 * sie gedrückt gehalten werden. Hierfür wird bei erkanntem Tastendruck einfach ein Flag gesetzt
 * und bei Loslassen aller Buttons wieder gelöscht. Wird ein Tastendruck erkannt, so lange das Flag
 * noch gesetzt ist, gehen wir davon aus, dass die AKtion schon ausgelöst, der Button aber noch nicht
 * losgelassen wurde.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  * GND -> Button -> Pin 7
 *  * GND -> Button -> Pin 8
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/
 * https://docs.arduino.cc/language-reference/en/functions/time/delay/
 */

constexpr int button_plus_pin  = 7;
constexpr int button_minus_pin = 8;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Bereit ...");

  pinMode(button_plus_pin, INPUT_PULLUP);
  pinMode(button_minus_pin, INPUT_PULLUP);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int counter = 0;
  static bool button_released = true;

  if (digitalRead(button_plus_pin) == LOW) {
    // Plus-Button: Counter hochzählen
    delay(100);

    if (digitalRead(button_plus_pin) == LOW && button_released) {
      button_released = false;

      counter++;
      Serial.println(counter);
    }
  } else if (digitalRead(button_minus_pin) == LOW) {
    // Minus-Button: Counter runterzählen
    delay(100);

    if (digitalRead(button_minus_pin) == LOW && button_released) {
      button_released = false;

      counter--;
      Serial.println(counter);
    }
  } else {
    button_released = true;
  }
}
