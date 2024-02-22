/**
 * Abstandssensor, Version 2: Struktur
 * ===================================
 *
 * In dieser Version verwenden wir eine einfache Struktur, um die beiden Pins für den
 * Abstandssensor zusammenzufassen. Sehr viel einfacher wird der Quellcode dadurch nicht.
 * Wir sind aber fast am Ziel. Die nächste Version baut auf dieser auf und bietet uns
 * eine schöne, gekappselte und wiederverwendbare Klasse.
 */

typedef struct {
  int trigger_pin;
  int echo_pin;
} distance_sensor_t;


distance_sensor_t sensor = {
  .trigger_pin = 23,
  .echo_pin    = 22,
};


long measure_distance_cm(distance_sensor_t& sensor) {
  digitalWrite(sensor.trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trigger_pin, LOW);

  auto duration = pulseIn(sensor.echo_pin, HIGH);
  return duration / 58.2;
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  pinMode(sensor.trigger_pin, OUTPUT);
  pinMode(sensor.echo_pin, INPUT);
}


void loop() {
  delay(1000);

  int distance_cm = measure_distance_cm(sensor);
  Serial.printf("Entfernung: %u cm\n", distance_cm);
}
