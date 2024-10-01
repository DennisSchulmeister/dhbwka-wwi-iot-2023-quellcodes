/**
 * Analogsensor auslesen
 * =====================
 *
 * Minimalbeispiel für das Messen einer analogen Spannung. Die Analogeingänge des
 * Arduino Uno können Spannung von 0V…5V mit 10 Bit Präzision messen und liefern
 * daher Werte von 0…1023. Diese werden hier an den Serial Plotter der Arduino IDE
 * gesendet.
 *
 * Hardwareaufbau:
 * ---------------
 *
 *   * Pin A0: KY-018 Helligkeitssensor oder anderen Spannungsteiler
 *
 * Der Spannungsteiler muss am einen Ende gegen 5V und am anderen Ende gegen
 * Masse/Ground verbunden werden. Der mittlere Ausgang, an dem die variable
 * Spannung anliegt, wird mit A0 verbunden, um die Spannung zu messen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://sensorkit.joy-it.net/de/sensors/ky-018
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
 */

constexpr int SENSOR = A0;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  Serial.begin(9600);
}

/**
 * Unendlich oft ausgeführte Programmlogik
 */
void loop() {
  int sensor_value    = analogRead(SENSOR);
  int sensor_inverted = 1023 - sensor_value;

  Serial.print("sensor_value:");
  Serial.print(sensor_value);
  Serial.print(",sensor_inverted:");
  Serial.print(sensor_inverted);
  Serial.print("\n");
}
