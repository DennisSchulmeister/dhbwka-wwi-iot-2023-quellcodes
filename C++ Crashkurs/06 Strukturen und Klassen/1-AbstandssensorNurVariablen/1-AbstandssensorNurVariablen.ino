/**
 * Abstandssensor, Version 1: Nur Variablen
 * ========================================
 *
 * Hier verwenden wir den Ultraschall-Abstandssensor des Sensor Kits, um den Abstand
 * zu einem Hinderniss zu messen. Der Einfachheit halber haben wir hierfür eine Funktion
 * mit zwei Parametern für die beiden Hardware-Pins definiert.
 */
 
constexpr int SENSOR_TRIGGER_PIN = 23;
constexpr int SENSOR_ECHO_PIN    = 22;


long measure_distance_cm(int trigger_pin, int echo_pin) {
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  auto duration = pulseIn(echo_pin, HIGH);
  return duration / 58.2;
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  pinMode(SENSOR_TRIGGER_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_PIN, INPUT);
}


void loop() {
  delay(1000);

  int distance_cm = measure_distance_cm(SENSOR_TRIGGER_PIN, SENSOR_ECHO_PIN);
  Serial.printf("Entfernung: %u cm\n", distance_cm);
}
