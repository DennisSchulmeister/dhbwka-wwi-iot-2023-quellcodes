/* Hallo serieller PLotter
 * =======================
 *
 * Dieses Beispiel misst eine analoge Spannung am Analogeingang A0 und stellt sie mit dem seriellen Plotter
 * der Arduino IDE grafisch dar. Hier wird jede halbe Sekunde ein Wert gemessen und im Format "Name:Wert"
 * auf der seriellen Konsole ausgegeben.
 *
 * HINWEISE:
 *
 *  - Alle Messwerte müssen in einer Zeile gesendet werden.
 *  - Die Zeile darf keine Leerzeichen beinhalten.
 *  - Vor jedem Messwert steht ein Name gefolgt von einem Doppelpunkt.
 *  - Mehrere Messwerte von unterschiedlichen Sensoren werden mit Komma getrennt.
 *
 * BEISPIEL:
 *
 *   Sensor_1:227,Sensor_2:412
 *   Sensor_1:235,Sensor_2:376
 *   Sensor_1:249,Sensor_2:342
 *   ...
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * - Arudino Dokumentation:  https://docs.arduino.cc/programming/
 * - Serielle Kommunikation: https://docs.arduino.cc/language-reference/en/functions/communication/serial/
 * - Funktion pinMode():     https://docs.arduino.cc/language-reference/en/functions/digital-io/pinmode/
 * - Funktion analogRead():  https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/
 * - Funktion delay():       https://docs.arduino.cc/language-reference/en/functions/time/delay/
 */

constexpr int sensor1_pin = A0;

/**
 * Initialisierungen bei Programmstart
 */
void setup() {
  delay(500);
  pinMode(sensor1_pin, INPUT);
  Serial.begin(9600);
}

/**
 * Als Endlosschleife laufende Hauptverarbeitungslogik
 */
void loop() {
  int value1 = analogRead(sensor1_pin);

  Serial.print("Sensor_1:");
  Serial.print(value1);
  Serial.print(",");
  Serial.print("Dummy_Wert:");
  Serial.print(47.11);
  Serial.print("\n");

  delay(500);
}
