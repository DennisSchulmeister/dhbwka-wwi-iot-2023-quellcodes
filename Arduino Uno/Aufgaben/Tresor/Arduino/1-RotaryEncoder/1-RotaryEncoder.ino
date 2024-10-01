/**
 * Drehimpulsregler (Rotary Encoder)
 * =================================
 *
 * Dieses Beispiel zeigt, wie ein "Rotary Encoder" (deutsch: Drehimpulsgeber) ausgelesen kann, um
 * unterschiedliche Arten von Drehbewegungen zu erkennen. Äußerlich sieht ein Drehimpulsgeber wie
 * ein Potentiometer aus, basiert jedoch nicht auf einem Spannungsteiler. Stattdessen besitzt er
 * eine Rasterung, die dafür sorgt, dass bei jeder Bewegung zwei Digitalimpulse generiert werden,
 * mit denen die Drehrichtung erkannt werden kann:
 *
 *                   1x nach links           1x nach rechts
 *                   CLK eilt voraus         DT eilt voraus
 *     CLK | … --------+        +--------- … ----------+        +-------- …
 *         |           +--------+                      +--------+
 *         |
 *      DT | … ------------+        +----- … -----+        +------------ …
 *         |               +--------+             +--------+
 *
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Die Schaltung können Sie aus den Konstanten unten ableiten. Es ist nicht schwer, da der Drehregler
 * direkt mit dem Arduino verbunden werden kann.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://sensorkit.joy-it.net/de/sensors/ky-040
 * https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
 */

constexpr int PIN_ENCODER_CLK = 4;
constexpr int PIN_ENCODER_DT  = 3;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(PIN_ENCODER_CLK, INPUT_PULLUP);
  pinMode(PIN_ENCODER_DT,  INPUT_PULLUP);

  Serial.begin(9600);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int clk_before = digitalRead(PIN_ENCODER_CLK);
  int clk = digitalRead(PIN_ENCODER_CLK);

  if (clk_before != clk) {
    clk_before = clk;

    if (clk == LOW) {
      if (digitalRead(PIN_ENCODER_DT) == LOW) {
        Serial.println("Links");
      } else {
        Serial.println("Rechts");
      }

      delay(100);
    }
  }
}
