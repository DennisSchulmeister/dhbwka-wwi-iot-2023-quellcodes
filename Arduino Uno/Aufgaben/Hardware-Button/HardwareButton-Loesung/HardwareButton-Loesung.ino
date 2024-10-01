/**
 * Hardware-Buttons entprellen (Musterlösung)
 * ==========================================
 *
 * Abgewandelte Version des Beispiels zum Entprellen von Hardware-Buttons. Im Original wird
 * die interne LED des Arduino so lange eingeschaltet, wie ein an Pin 1 angeschlossener Button
 * gedrückt wird. Diese Version verwendet zwei Buttons, von denen einer die LED ein- und der
 * andere sie wieder ausschaltet. Wird kein Button gedrückt, behält die LED ihren letzten Zustand.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *  - [PIN 2] --> [Erster Button] --> [GND]
 *  - [PIN 3] --> [Zweiter Button] --> [GND]
 *  - Optional: [PIN 13] --> [LED] --> [Widerstand 300 Ohm] --> [GND]
 *
 * Für die Button-Pins wird der eingebaute Pull-Up-Widerstand des Microcontrollers
 * aktiviert. Die Buttons müssen das Signal daher beim Drücken auf Ground runterziehen.
 *
 * Die LED ist optional, da wir hier einfach die sowieseo eingebaute LED auf
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

#include "DebouncedDigitalInput.h"

auto button1  = DebouncedDigitalInput(2, true, 100);
auto button2  = DebouncedDigitalInput(3, true, 100);
bool ledValue = false;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  button1.setup();
  button2.setup();

  // Warten, bis sich die GPIOs stabilisiert haben
  delay(1000);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  if (button1.digitalRead() == LOW) {
    ledValue = true;
  } else if (button2.digitalRead() == LOW) {
    ledValue = false;
  }

  digitalWrite(LED_BUILTIN, ledValue);
}
