/* Hallo serielle Konsole
 * ======================
 *
 * Dies ist ein winzig kleines Einstiegsbeispiel in die Arduino-Programmierung. Es zeigt den allgemeinen
 * Aufbau eines Arduino-Programms und ein paar einfache Dinge wie den Austausch serieller Daten mit dem
 * Host-Computer und die Ansteuerung der eingebauten LED des Arduino-Boards.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * - Arudino Dokumentation:   https://docs.arduino.cc/programming/
 * - Serielle Kommunikation:  https://docs.arduino.cc/language-reference/en/functions/communication/serial/
 * - String-Objekte:          https://docs.arduino.cc/language-reference/en/variables/data-types/stringobject/
 * - Funktion pinMode():      https://docs.arduino.cc/language-reference/en/functions/digital-io/pinmode/
 * - Funktion digitalWrite(): https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/
 * - Funktion delay():        https://docs.arduino.cc/language-reference/en/functions/time/delay/
 */

/**
 * Eingabaute setup()-Funktion
 * ---------------------------
 *
 * Diese Funktion wird direkt nach dem Einschalten des Arduino ausgeführt, um das Programm
 * und die angeschlossenen Hardwarebausteine zu initialisieren. Hier legen wir fest, dass
 * der Ausgangspin, an dem die eingebaute LED hängt, als Ausgang verwendet werden soll,
 * initialisieren die serielle Kommunikation mit dem Host-Computer und führen ein kurzes
 * Begrüßungsgespräch mit der Person am Bildschirm.
 */
void setup() {
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  String name = serial_input("Wie heißt du?");

  Serial.print("\n");
  Serial.print("Hallo ");
  Serial.print(name.c_str());
  Serial.print("!\n");
}

/**
 * Eingebaute loop()-Funktion
 * --------------------------
 *
 * Diese Funktion wird nach der setup()-Funktion in einer Endlosschleife unendlich oft
 * ausgeführt. Typischerweise würde man hier daher die elektrischen Eingänge abfragen,
 * die ermittelten Werte interpretieren und dann die Ausgänge steuern, um eine Aktion in
 * der realen Welt auszulösen. Hier sparen wir uns die ersten beiden Schritte und lassen
 * einfach die eingebaute LED des Arduinos blinken.
 */
void loop() {
  Serial.println("Schalte LED an ...");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

  Serial.println("Schalte LED aus ...");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

/**
 * Dies ist ein Beispiel für eine selbst-definierte Hilfsfunktion. Sie gibt einen Text auf
 * der seriellen Konsole aus und hält das Programm an, bis eine Antwort eingegeben und mit
 * ENTER bestätigt wurde. ACHTUNG: Die Eingabe muss im Eingabefeld über der Konsolenausgabe
 * gemacht werden. :-)
 */
String serial_input(const char* prompt) {
  Serial.print(prompt);
  String result = {};
  
  while (result.length() == 0) {
    result = Serial.readStringUntil('\n');
    result.trim();
  }
  
  return result;
}
