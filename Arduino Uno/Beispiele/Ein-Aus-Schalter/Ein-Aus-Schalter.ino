/**
 * Ein- und Ausschalter
 * ====================
 *
 * Dieser Sketch kann für die ersten Hands-On-Versuche mit Hardwareschaltungen am Arduino verwendet werden.
 * Drei an die Pins 5, 6 und 7 angeschlossene Buttons mit Active-Low-Logik schalten drei LEDs oder andere
 * Verbraucher mit Active-High-Logik an den Pins 11, 12 und 13 ein oder aus.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Pin 5  --> Button --> GND, Steuert Ausgangspin 11
 * Pin 6  --> Button --> GND, Steuert Ausgangspin 12
 * Pin 7  --> Button --> GND, Steuert Ausgangspin 13
 *
 * Pin 11 --> 300 Ohm Widerstand --> LED --> GND
 * Pin 12 --> 300 Ohm Widerstand --> LED --> GND
 * Pin 13 --> 300 Ohm Widerstand --> LED --> GND
 *
 * Die Buttons an Pin 5 und 6 schalten ihren Verbraucher dauerhaft ein oder aus. Der Button an Pin 7 schaltet
 * den Verbraucher nur so lange, wie der Button gedrückt gehalten wird. Die Widerstände können vor oder hinter
 * den LEDs eingebaut werden.
 *
 *
 * Aufgaben
 * --------
 *
 * 1) Bauen Sie den Hardwareaufbau nach und bringen Sie das Beispiel zum Laufen.
 * 2) Messen Sie die Eingangsspannung an den Pins 5 bis 7, wenn kein Button gedrückt wird.
 * 3) Messen Sie die Eingangsspannung an den Pins 5 bis 7, während ein Button gedrückt wird.
 * 4) Messen Sie die Ausgangsspannung an den Pins 11 bis 13, wenn die LEDs ausgeschaltet sind.
 * 5) Messen Sie die Ausgangsspannung an den Pins 11 bis 13, wenn die LEDs eingeschaltet sind.
 * 6) Messen Sie den Spannungsabfall entlang einer LED und entlang ihres Vorwiderstands.
 * 7) Messen Sie die Stromstärke des Stroms, der die LED leuchten lässt.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
 */

constexpr int pin_button1 = 5;
constexpr int pin_button2 = 6;
constexpr int pin_button3 = 7;

constexpr int pin_output1 = 11;
constexpr int pin_output2 = 12;
constexpr int pin_output3 = 13;

constexpr int debounce_ms = 50;


/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(pin_button1, INPUT_PULLUP);
  pinMode(pin_button2, INPUT_PULLUP);
  pinMode(pin_button3, INPUT_PULLUP);

  pinMode(pin_output1, OUTPUT);
  pinMode(pin_output2, OUTPUT);
  pinMode(pin_output3, OUTPUT);

  digitalWrite(pin_output1, LOW);
  digitalWrite(pin_output2, LOW);
  digitalWrite(pin_output3, LOW);

  delay(1000);
  Serial.begin(9600);
  Serial.println("Bereit ...");
}

/**
 * Hilfsfunktion zum Erkennen eines kurzen Tastendrucks
 */
int readButton(int pin, bool* value) {
  if (*value == false && digitalRead(pin) == LOW) {
    delay(debounce_ms);

    if (digitalRead(pin) == LOW) {
      *value = true;

      Serial.print("Button an Pin ");
      Serial.print(pin);
      Serial.println(" wurde gedrückt");
    }
  } else if (*value == true && digitalRead(pin) == HIGH) {
    delay(debounce_ms);

    if (digitalRead(pin) == HIGH) {
      *value = false;

      Serial.print("Button an Pin ");
      Serial.print(pin);
      Serial.println(" wurde losgelassen");
    }
  }
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static bool button1 = false;
  static bool button2 = false;

  static bool toggle1 = false;
  static bool toggle2 = false;

  static bool output1 = LOW;
  static bool output2 = LOW;
  static bool output3 = LOW;

  readButton(pin_button1, &button1);
  readButton(pin_button2, &button2);

  if (button1 && toggle1) {
    output1 = !output1;
    toggle1 = false;
  } else if (!button1) {
    toggle1 = true;
  }

  if (button2 && toggle2) {
    output2 = !output2;
    toggle2 = false;
  } else if (!button2) {
    toggle2 = true;
  }

  output3 = !digitalRead(pin_button3);
  
  digitalWrite(pin_output1, output1);
  digitalWrite(pin_output2, output2);
  digitalWrite(pin_output3, output3);  
}