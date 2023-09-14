/**
 * Einparkhilfe
 * ============
 *
 * Fallbeispiel für eine einfache Einparkhilfe, wie sie in Autos verbaut wird. Wird ein elektrischer
 * Kontakt geschlossen, während der Rückwärtsgang eingelegt ist, findet über einen Ultraschallsensor
 * eine regelmäßige Abstandsmessung statt. Die gemessene Entfernung wird durch einen periodischen
 * Signalton hörbar gemacht. Je kleiner der Abstand, desto schneller die Signaltöne bis hin zu einem
 * Dauerton bei Unterschreiten der Mindestdistanz.
 *
 * Das Beispiel zeigt auch die Verwendung eigener Hilfsfunktionen, um den Quellcode übersichtlicher
 * und leserlicher zu gestalten. Das Besondere an den Funktionen ist, dass einige davon mit static
 * statische Variablen definieren, die ihre Werte nicht verlieren, sondern beim nächsten Aufruf
 * derselben Funktion beibehalten. Dadurch sparen wir uns, die Funktionalität in Klassen zu kapseln,
 * erkaufen dies aber mit einem höheren RAM-Bedarf und weniger Flexibilität.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Hardware-Button:
 *
 *    [Arduino Pin 2] --> [Button Pin 1]
 *     [Button Pin 2] --> [Arduino GND]
 *
 * Ultraschallsensor:
 *
 *   [Arduino Pin 3] --> [Sensor Trigger]
 *     [Sensor Echo] --> [Arduino Pin 4]
 *      [Sensor VCC] --> [Arduino 5V]
 *      [Sensor GND] --> [Arduino GND]
 *
 * Aktiver Buzzer / Signalgeber:
 *
 *   [Arduino Pin 5] --> [Buzzer SIGNAL]
 *      [Buzzer GND] --> [Arduino GND]
 *
 *   Falls statt einem aktiven Piezo-Buzzer eine LED angeschlossen wird, sollte ein Widerstand
 *   in Reihe eingebaut werden, um die Stromstärke zu besgrenzen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://sensorkit.joy-it.net/de/sensors/ky-050
 * https://sensorkit.joy-it.net/de/sensors/ky-006
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
 * https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
 * https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/
 */

const int BUTTON_PIN         = 2;
const int SENSOR_TRIGGER_PIN = 3;
const int SENSOR_ECHO_PIN    = 4;
const int BUZZER_PIN         = 5;
const int LED_PIN            = LED_BUILTIN;

const int   DISTANCE_BUFFER    = 0;
const float DISTANCE_MIN_CM    = 3.0;
const float DISTANCE_MAX_CM    = 30.0;
const float FREQUENCY_MIN_HZ   = 1.0;
const float FREQUENCY_MAX_HZ   = 8.00;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SENSOR_TRIGGER_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  float output_frequency_hz = 0.0;

  if (read_button(BUTTON_PIN) == LOW) {
    auto measured_distance_cm = measure_distance_cm(SENSOR_TRIGGER_PIN, SENSOR_ECHO_PIN);
    auto smoothed_distance_cm = calc_running_average(DISTANCE_BUFFER, measured_distance_cm);
  
    if (smoothed_distance_cm <= DISTANCE_MIN_CM) {
      output_frequency_hz = -1.0;
    } else if (smoothed_distance_cm <= DISTANCE_MAX_CM) {
      float percent = (smoothed_distance_cm - DISTANCE_MIN_CM) / (DISTANCE_MAX_CM - DISTANCE_MIN_CM);
      output_frequency_hz = ((1 - percent) * (FREQUENCY_MAX_HZ - FREQUENCY_MIN_HZ)) + FREQUENCY_MIN_HZ;
    }
  }

  auto output_signal = square_wave_signal(output_frequency_hz);
  digitalWrite(BUZZER_PIN, output_signal);
  digitalWrite(LED_PIN, output_signal);
}

/**
 * Hilfsfunktion zum Auslesen des Buttons mit einer minimalen Entprellung. Wird eine Änderung
 * des Eingangssignals erkannt, pausiert der Microcontroller für 10ms und prüft das Signal
 * so lange erneut, bis es stabil bleibt.
 *
 * Diese Funktion ist nur für nicht-zeitkritische Anwendungen geeignet, bei denen es kein Problem
 * darstellt, wenn der Programmfluss für eine unbestimmte Zeit pausiert wird, bis sich das Signal
 * am Eingang stabilisiert. Für zeitkritische Anwendungen siehe das gesonderte Beispiel zur Entprellung
 * von Hardwarebuttons in den Quellcodes zur Vorlesung.
 *
 * Parameter:
 *   - pin: Eingangspin des Buttons (max. 20)
 *
 * Rückgabe: Entprellter Wert
 */
bool read_button(int pin) {
  static bool debounced_value[20];
  auto current_value = digitalRead(pin);

  while (current_value != debounced_value[pin]) {
    delay(10);
    debounced_value[pin] = current_value;
    current_value = digitalRead(pin);
  }

  return current_value;
}

/**
 * Hilfsfunktion zum Durchführen einer Abstandsmessung. Der Sensor wird aktiviert, indem auf dem
 * Triggerpin für 10 Microsekunden ein Signal angelegt wird, wodurch ein akkustisches Ultraschallsignal
 * wiedergegeben wird. Anschließend wird die Zeit gemessen, bis das Signal von einem Hinderniss
 * reflektiert und an das Mikrofron am Echopin zurückgegeben wird. Anhand der Schallgeschwindigkeit
 * kann daraus dann die Entfernung ausgerechnet werden.
 *
 * Parameter:
 *  - trigger_pin: Ausgangspin zum Aktivieren des Sensors
 *  - echo_pin: Eingangspin für das reflektierte Ultraschallsignal
 *
 * Rückgabe: Gemessene Entfernung in cm
 */
long measure_distance_cm(int trigger_pin, int echo_pin) {
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  auto duration = pulseIn(echo_pin, HIGH);
  return duration / 58.2;
}

/**
 * Hilfsfunktion zur Glättung der Messwerte. Aufgrund von Reflexionen (Echos) des Ultraschallsignals
 * schwankt die gemessene Distanz immer wieder ein wenig. Diese Funktion kann für bis zu fünf Signale
 * einen laufenden Durchschnitt berechnen, um die Werte zu glätten. Dabei wird immer der Durchschnitt
 * aus den letzten fünf Messungen gebildet. Beide Zahlen können über die Konstanten am Anfang der
 * Funktion angepasst werden.
 *
 * Parameter:
 *  - id: Kennung des Messwerts (0 bis 4)
 *  - value: Zuletzt gemessener Wert
 *
 * Rückgabe: Gleitender Durchschnitt
 */
long calc_running_average(unsigned int id, long value) {
  const int N_BUFFERS = 5;
  const int N_VALUES  = 5;

  static float buffer[N_BUFFERS][N_VALUES];
  static unsigned int index[N_BUFFERS];

  index[id] = ++index[id] % N_BUFFERS;
  buffer[id][index[id]] = value;
  float average = 0;

  for (int i = 0; i < N_VALUES; i++) {
    average += buffer[id][i];
  }

  return average / N_VALUES;
}

/**
 * Hilfsfunktion zur Erzeugung eines Rechtecksignals zum Ansteuern des Buzzers. Diese Funktion muss
 * in loop() periodisch aufgerufen werden, um den Wert zu ermitteln, der über den Buzzer-Pin ausgegeben
 * werden muss (LOW oder HIGH). Hierfür bekommt die Funktion eine Frequenz in Hertz übergeben, z.B.
 * 3.0 für drei Signaltöne je Sekunde. Daraus generiert sie eine Rechteckform mit entsprechend drei
 * Phasendurchläufen je Sekunde.
 *
 * Sonderfälle:
 *
 *  - Frequenz = 0.0: Signal ist immer LOW (aus)
 *  - Frequenz < 0.0: Signal ist immer HIGH (Dauerton)
 *
 * Parameter:
 *  - frequency_hz: Anzahl der Signaltöne je Sekunde
 *
 * Rückgabe: Neuer Wert für den Ausgangspin
 */
bool square_wave_signal(float frequency_hz) {
  if (frequency_hz  < 0.0) return HIGH;
  if (frequency_hz == 0.0) return LOW;

  static bool signal = LOW;
  static unsigned long last_change_millis = 0;

  unsigned long now = millis();
  float half_period_ms = ((1.0 / frequency_hz) / 2.0 * 1000);
  if ((now - last_change_millis) >= half_period_ms) signal = !signal;

  return signal;
}
