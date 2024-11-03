/**
 * Nachtlicht
 * ==========
 *
 * Musterlösung zur Aufgabe "Automatisches Nachtlicht" im Folienskript. Das Programm nutzt Pulsweitenmodulation
 * um die Helligkeit einer LED in Abhängigkeit von der Umgebungshelligkeit einstellen. Zusützlich kann die LED
 * über einen Button auf maximall hell eingestellt werden.
 *
 * HINWEIS: Der Sensor liefert eine umso größere Spannung, je dunkler es ist!
 *
 * HINWEIS: Die Buttons werden hier mit der delay()-Funktion rudimentär entprellt. Jedoch wird, um nicht vom
 * Analogsensor abzulenken, nicht geprüft, ob ein Button auch tatsächlich wieder losgelassen wurde, bevor eine
 * Aktion ausgelöst wird. In einem echten Programm sollte man das vor allem bei dem Toggle-Button machen, der
 * bei erkanntem Tastendruck den aktuellen Modus wechselt.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * - KY-018 Fotowiderstand an Analogpin A0
 * - LED mit Vorwiderstand an Digtialpin ~3
 * - Button, der gegen GND schaltet, an Digitalpin 4 
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/
 * https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/
 * https://docs.arduino.cc/language-reference/en/functions/analog-io/analogWrite/
 * https://sensorkit.joy-it.net/de/sensors/ky-018
 */

constexpr int ambience_sensor_pin = A0;
constexpr int led_output_pin = 3;
constexpr int button_manual_mode_pin = 4;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(ambience_sensor_pin, INPUT);
  pinMode(led_output_pin, OUTPUT);
  pinMode(button_manual_mode_pin, INPUT_PULLUP);

  Serial.begin(9600);
  delay(1000);
  Serial.println("Bereit ...");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  // Fotowiderstand auslesen
  int ambience_sensor_value = analogRead(ambience_sensor_pin);

  // Button abfragen
  static bool manual_mode = false;

  if (!digitalRead(button_manual_mode_pin)) {
    delay(100);
    if (!digitalRead(button_manual_mode_pin)) manual_mode = !manual_mode;
    
    Serial.print("Manueller Modus: ");
    Serial.println(manual_mode);

    delay(1000);
  }

  // LED ansteuern
  if (manual_mode) {
    digitalWrite(led_output_pin, HIGH);
  } else {
    int led_output_pwm = ambience_sensor_value / 1024.0 * 255;
    analogWrite(led_output_pin, led_output_pwm);

    Serial.print("Automatik-Modus | ");
    Serial.print("Sensor: ");
    Serial.print(ambience_sensor_value);
    Serial.print(" | LED: ");
    Serial.println(led_output_pwm);

    delay(250);
  }
}
