/**
 * Analogeingang
 * =============
 *
 * Dieser Sketch ermöglicht die Messung analoger Spannungswerte am Pin A0, um damit
 * verschiedene Hardwarebschaltungen auszuprobieren (siehe beigelegte Hardwareskizzen):
 *
 *   * Drehpotentionemeter
 *   * Helligkeitssenser
 *   * Mehrere Buttons
 *
 * Sie alle beruhen auf dem Prinzip des Spannungsteilers. Die 5V Versorgungsspannung
 * des Arduino wird durch zwei oder mehrere in Reihe geschaltete Widerstände in kleinere
 * Teilspannungen, die sich als Analogsignal messen lassen, zerlegt.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Siehe Hardwareskizzen. Der Spannungsteiler muss am einen Ende mit 5V und am anderen
 * Ende mit GND verbunden werden. In der Mitte wird die zu messende Spannung abgegriffen
 * und dem Arduino an Pin A0 zugeführt.
 *
 * Aufgaben
 * --------
 *
 * 1) Drehpoti: Messen des Gesamtwiderstands mit einem Multimeter
 * 2) Drehpoti: Messen des Teilwiderstands mit einem Multimeter
 * 3) Drehpoti: Messen der Teilspannung mit einem Multimeter
 * 4) Drehpoti: Messen der Teilspannung mit dem Arduino
 * 5) Drehpoti: Visualisieren der Teilspannung mit einem Oszilloskop
 *
 * 5) Helligkeitssensor: Messen der Helligkeit mit dem Arduino
 *
 * 6) Mehrere Buttons: Messen der Teilspannungen mit dem Arduino
 *
 * Versuchen Sie in allen Fällen, die verschiedene Werte zu messen durch Drehen am Poti,
 * Verdecken des Helligkeitssensors bzw. drücken der Buttons.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
 * https://sensorkit.joy-it.net/de/sensors/ky-018
 * https://sensorkit.joy-it.net/de/sensors/ky-054
 */

constexpr int pin = A0;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(pin, INPUT);

  delay(1000);
  Serial.begin(9600);
  Serial.println("Bereit ...");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int prev_value = -1;
  int value = analogRead(pin);

  if (abs(value - prev_value) > 5) {
    prev_value = value;

    float volts = value * 5.0 / 1023.0;

    Serial.print("Messwert: ");
    Serial.print(value);
    Serial.print(" (");
    Serial.print(volts);
    Serial.println(" V)");
  }
}
