/**
 * Hardware-Buttons entprellen
 * ===========================
 *
 * Arduino macht es leicht, mit der Funktion digitalRead() ein digitales Eingangssignal
 * auszulesen. Mechanische Bauteile wie z.B. Hardware-Buttons können aber nicht einfach
 * den Zustand von an nach aus oder umgekehrt wechseln. Sie pendeln bei jedem Wechseln
 * eine Weile zwischen beiden Zuständen hin und her, bis der mechanische Kontakt sicher
 * hergestellt wurde.
 *
 * Idealzustand, den wir gerne hätten:
 *
 *   Signal
 *    |...+                   +...
 *    |   |                   |
 *    |   |                   |
 *    |   +...................+
 *    +--------------------------> Zeit
 *
 * Reale Situation:
 *
 *   Signal
 *    |....+  +..+  +..+      +..
 *    |    |  |  |  |  |      |
 *    |    |  |  |  |  |      |
 *    |    +..+  +..+  +......+..
 *    +--------------------------> Zeit
 *
 * Mechanische Bauteile müssen daher "entprellt" werden (engl. "debounce"), indem bei
 * einem Wechsel des Signalzustands abgewartet wird, bis sich das Signal beruhigt hat.
 * Hierfür muss bei jedem registrierten Wechsel geprüft werden, wie lange der letzte
 * Wechsel her ist. Ist der zeitliche Abstand zum letzten Signalwechsel groß genug,
 * darf auf den neue Zustand reagiert werden.
 *
 * Hierfür gibt es in der Arduino IDE ein eingebautes Beispiel, welches das Prinzip
 * verdeutlicht. Um den Code jedoch zu vereinfachen und leichter wiederverwenden zu
 * können (bspw. wenn mehrere Buttons abgefragt werden sollen), wurde die Logik zum
 * Abfragen und Entprellen des Eingangspin hier in einer Klasse gekapselt.
 *
 * Das Programm fragt einfach einen an Pin 2 angeschlossenen Button ab, der auf Ground
 * durchschalten muss, wenn er gedrückt wird, und schaltet die interne LED des Arduino
 * entsprechend ein oder aus.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  - [PIN 2] --> [Button] --> [GND]
 *  - Optional: [PIN 13] --> [LED] --> [Widerstand 300 Ohm] --> [GND]
 *
 * Für den Button-Pin wird der eingebaute Pull-Up-Widerstand des Microcontrollers
 * aktiviert. Der Button muss das Signal daher beim Drücken auf Ground runterziehen,
 * wodurch sich die Logik im Programm invertiert: Wir lesen ein LOW, wenn der Button
 * gedrückt wird und ein HIGH, wenn er nicht gedrückt wird.
 *
 * Die LED ist optional, da wir hier einfach die sowieso eingebaute LED auf
 * dem Arduino Board verwenden. Falls das X40 Sensor Kit verwendet wird, wird
 * kein Widerstand für die LED benötigt, da dieser bereits auf dem LED-Modul
 * enthalten ist. Andernfalls kann ein beliebiger Widerstand ab ca. 300 Ohm
 * verwendet werden, um die Stromstärke für die LED zu begrenzen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/learn/microcontrollers/digital-pins
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
 * https://www.arduino.cc/reference/en/language/functions/time/millis/
 */

#include "lib/DebouncedDigitalInput.h"

auto button = DebouncedDigitalInput(2, true, 100);

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  button.setup();
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  digitalWrite(LED_BUILTIN, !button.digitalRead());
}
