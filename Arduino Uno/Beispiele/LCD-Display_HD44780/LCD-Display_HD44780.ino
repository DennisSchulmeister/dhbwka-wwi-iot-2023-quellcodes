/**
 * LCD-Display mit HD44780-Controller
 * ==================================
 *
 * Der in den 1980er-Jahren auf den Markt gebrachte Display-Controller HD44780 gilt als Industriestandard
 * zur Anbindung zeichenbasierter Dot-Matrix-Displays. Leider bindet sein paralleler, auf die Bussysteme
 * damaliger Computer zugeschnittente Datenaustausch aber auch viele Pins am Arduino. Dieses Beispiel
 * zeigt, wie ein solches Display zur Implementierung einer einfachen Statusanzeige angebunden werden kann.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Siehe Zeilen 24 - 33 unten. RS, RE, D0, ... sind die Anschlüsse am Display. Die Zahlen rechts davon
 * die Pins am Arduino Uno. Die anderen Anschlüsse des Display werden wie folgt verbunden:
 *
 *  - VSS -> Ground
 *  - VDD -> +5V
 *  - RW  -> Ground
 *  - A   -> +5V
 *  - K   -> Ground
 *
 * Damit man das Display lesen kann, muss VEE mit einem Potentiometer zum Einstellen einer variablen
 * Spannung verbunden werden. Das Poti muss dann so eingestellt werden, dass die Schrift auf dem
 * Display lesbar ist.
 *
 *              +5V
 *               ⦚
 *  - VEE ------>⦚
 *               ⦚
 *            Ground
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/libraries/liquidcrystal/
 */

#include <LiquidCrystal.h>

LiquidCrystal lcd(
  /* RS */ 2,
  /* RE */ 3,
  /* D0 */ 4,
  /* D1 */ 5,
  /* D2 */ 6,
  /* D3 */ 7,
  /* D4 */ 8,
  /* D5 */ 9,
  /* D6 */ 10,
  /* D7 */ 11
);

constexpr int button1_pin = 12;
constexpr int button2_pin = 13;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);

  lcd.begin(16, 2);

  while (!Serial);
  Serial.begin(9600);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  // Buttons abfragen
  static bool buttons_released = true;
  static int current_message = 0;
  static int prev_message = -1;
  int number_messages = 3;

  if (digitalRead(button1_pin) == LOW) {
    delay(100);

    if (buttons_released && digitalRead(button1_pin) == LOW) {
      buttons_released = false;

      current_message = (current_message + 1) % number_messages;
    }
  } else if (digitalRead(button2_pin) == LOW) {
    delay(100);

    if (buttons_released && digitalRead(button2_pin) == LOW) {
      buttons_released = false;

      current_message--;
      if (current_message < 0) current_message = number_messages - 1;
    }
  } else {
    buttons_released = true;
  }

  // Ausgewählte Meldung anzeigen
  if (current_message != prev_message) {
    prev_message = current_message;
    lcd.clear();

    switch (current_message) {
      case 0:
        Serial.println("Statusmeldung 1");
        lcd.print("Statusmeldung 1");
        break;
      case 1:
        Serial.println("Statusmeldung 2");
        lcd.print("Statusmeldung 2");
        break;
      case 2:
        Serial.println("Statusmeldung 3");
        lcd.print("Statusmeldung 3");
        break;
    }
  }
}
