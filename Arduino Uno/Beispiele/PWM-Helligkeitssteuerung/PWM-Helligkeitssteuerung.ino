/**
 * PWM-Helligkeitssteuerung
 * ========================
 *
 * Dieser Sketch zeigt, zwei ein pulsweitenmoduliertes Signal verwendet werden kann, um die mittlere
 * Leistung eines angeschlossenen Verbrauchers zu regulieren. Hierzu wird hier die Helligkeit einer
 * LED mit drei Buttons geregelt.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Pin 5  --> Button --> GND, Steuert Ausgangspin 11
 * Pin 6  --> Button --> GND, Steuert Ausgangspin 12
 * Pin 7  --> Button --> GND, Steuert Ausgangspin 13
 * Pin 11 --> 300 Ohm Widerstand --> LED --> GND
 *
 * Die Buttons an Pin 5 und 6 regeln die Helligkeit der LED heller bzw. dunkler. Pin 7 stellt die
 * Helligkeit auf mittlere Stufe. Der Wiederstand zur Begrenzung der Stromstärke kann dabei vor oder
 * hinter der LED eingebaut werden.
 *
 * Aufgaben
 * --------
 *
 * 1) Bauen Sie den Hardwareaufbau nach und bringen Sie das Beispiel zum Laufen.
 * 2) Versuchen Sie, mit den Buttons die LED heller oder dunker zu stellen.
 * 3) Messen Sie die Ausgangsspannung am Pin 11 für verschiedene Helligkeiten.
 * 4) Visualisieren Sie das Ausgangssignal an Pin 11 mit einem Oszilloskop.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
 */

constexpr int pin_button1 = 5;
constexpr int pin_button2 = 6;
constexpr int pin_button3 = 7;
constexpr int pin_output  = 11;
constexpr int debounce_ms = 50;

constexpr uint8_t default_value = 120;
constexpr uint8_t delta = 10;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(pin_button1, INPUT_PULLUP);
  pinMode(pin_button2, INPUT_PULLUP);
  pinMode(pin_button3, INPUT_PULLUP);

  pinMode(pin_output, OUTPUT);
  analogWrite(pin_output, default_value);

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
  static bool button3 = false;

  static bool toggle1 = false;
  static bool toggle2 = false;
  static bool toggle3 = false;

  static uint8_t value = default_value;

  readButton(pin_button1, &button1);
  readButton(pin_button2, &button2);
  readButton(pin_button3, &button3);

  if (button1 && toggle1) {
    if (value <= (255 - delta)) value += delta;
    toggle1 = false;
  } else if (!button1) {
    toggle1 = true;
  }

  if (button2 && toggle2) {
    if (value >= delta) value -= delta;
    toggle2 = false;
  } else if (!button2) {
    toggle2 = true;
  }

  if (button3 && toggle3) {
    value = default_value;
    toggle3 = false;
  } else if (!button3) {
    toggle3 = true;
  }

  analogWrite(pin_output, value);
}
