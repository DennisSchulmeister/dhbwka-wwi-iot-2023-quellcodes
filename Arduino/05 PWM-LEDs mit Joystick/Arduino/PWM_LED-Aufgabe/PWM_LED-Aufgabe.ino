/**
 * PWM-Gesteuerte LEDs
 * ===================
 *
 * In dieser Aufgabe sollen Sie eine kleine Hardwareschaltung entwerfen und bauen, die es
 * ermöglicht, die beiden Achsen und den Button des im Sensor Kit enthaltenen Joysticks
 * als Analogsignal auszulesen. Die Helligkeit zweier LEDs soll damit gesteuert werden.
 *
 * Joystick-Button nicht gedrückt: Die beiden Achsen des Joysticks steuern jeweils die
 * Helligkeit einer der beiden LEDs. Die X-Achse steuert also LED1 und die Y-Achse LED2.
 * Beachten Sie hierbei, dass analogRead() einen 10-Bit Wert liefert, analogWrite() aber
 * nur mit 8 Bit arbeitet.
 *
 * Joystick-Button gedrückt: Die beiden LEDs leuchten mit maximaler Stärke.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Die Schaltung müssen Sie selbst erarbeiten. Es ist aber nicht schwer, da der Joystick
 * direkt und die LEDs mit je einem Vorwiderstand verbunden werden können.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://sensorkit.joy-it.net/de/sensors/ky-023
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
 */


/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
}
