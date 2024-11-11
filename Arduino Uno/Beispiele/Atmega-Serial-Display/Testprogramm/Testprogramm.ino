/**
 * Testprogramm für Atmega-Display-Board
 * =====================================
 *
 * Dies ist ein kleines Testprogramm für beliebige Arduino-Boards, um damit unser selbst-gebautes
 * Atmega 328p Display-Board zu testen. Das Programm gibt einen Zähler auf dem Display aus, der
 * mit dem Drehregler verändert und per Knopfdruck bestätigt werden kann.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Das Display-Board muss mit Strom versorgt und mit dem UART des Arduino verbunden werden.
 *
 *  - 0 RX -> Display Board TX
 *  - 1 TX -> Display Board RX
 *  - GND --> Display Board GND
 *  - 5V ---> Display Board 5V
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/communication/Serial/
 */

// Konstanten für empfangende Nachrichten
constexpr char ENCODER_LEFT   = 'l';
constexpr char ENCODER_RIGHT  = 'r';
constexpr char BUTTON_PRESSED = 'b';

// Konstanten für gesendete Nachrichten
constexpr char INIT_DISPLAY   = 'I';
constexpr char CLEAR_SCREEN   = 'C';
constexpr char LOCATE         = 'L';
constexpr char PRINT          = 'P';

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  while (!Serial);
  Serial.begin(9600);

  // Etwas warten, bis das Display-Board wirklich bereit ist
  delay(500);

  // Display initialisieren mit 16x2 Zeichen
  Serial.write(INIT_DISPLAY);
  Serial.write(16);   // Anzahl Spalten
  Serial.write(2);    // Anzahl Zeilen
  Serial.flush();
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int counter = 0;
  static int prevCounter = -1;

  while (Serial.available()) {
    char msg = Serial.read();

    switch (msg) {
      case ENCODER_LEFT:
        counter--;
        break;
      case ENCODER_RIGHT:
        counter++;
        break;
      case BUTTON_PRESSED:
        Serial.write(CLEAR_SCREEN);

        Serial.write(LOCATE);
        Serial.write(6);    // Spalte
        Serial.write(0);    // Zeile

        Serial.write(PRINT);
        Serial.println("Okay");

        Serial.flush();
        delay(500);
    }
  }

  if (counter != prevCounter) {
    prevCounter = counter;

    Serial.write(CLEAR_SCREEN);

    Serial.write(LOCATE);
    Serial.write(0);    // Spalte
    Serial.write(0);    // Zeile

    Serial.write(PRINT);
    Serial.println("Zähler:");

    Serial.write(LOCATE);
    Serial.write(0);    // Spalte
    Serial.write(1);    // Zeile

    Serial.write(PRINT);
    Serial.println(counter);

    // Test des Sonderzeichen-Mappings
    Serial.write(LOCATE);
    Serial.write(6);    // Spalte
    Serial.write(1);    // Zeile

    Serial.write(PRINT);
    Serial.println("ÄÖÜäöü←→~\\");
  }
}
