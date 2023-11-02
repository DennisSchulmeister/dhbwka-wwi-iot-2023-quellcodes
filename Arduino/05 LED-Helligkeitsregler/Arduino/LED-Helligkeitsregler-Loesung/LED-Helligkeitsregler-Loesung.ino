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
constexpr int PIN_LED1 = 5;
constexpr int PIN_LED2 = 6;

constexpr int PIN_JOYSTICK_BTN = 7;
constexpr int PIN_JOYSTICK_X   = A0;
constexpr int PIN_JOYSTICK_Y   = A1;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_JOYSTICK_BTN, INPUT_PULLUP);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  int joystick_x   = analogRead(PIN_JOYSTICK_X);
  int joystick_y   = analogRead(PIN_JOYSTICK_Y);
  int joystick_btn = digitalRead(PIN_JOYSTICK_BTN);

  if (joystick_btn == LOW) {
    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, HIGH);
  } else {
    analogWrite(PIN_LED1, joystick_x >> 2);
    analogWrite(PIN_LED2, joystick_y >> 2);
  }
}
