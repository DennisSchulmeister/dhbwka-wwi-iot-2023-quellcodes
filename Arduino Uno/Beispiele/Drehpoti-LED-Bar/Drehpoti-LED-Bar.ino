/**
 * LED-Bar mit Drehpoti
 * ====================
 *
 * Dieser Sketch misst eine Analogspannung z.B. von einem Drehpoti und steuert damit eine
 * kleine LED-Bar mit vier Leuchtdioden an. Je höher die gemessene Spannung ist, desto
 * mehr LEDs leuchten.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * 5V --> Drehpoti --> GND
 *            |
 *            -------> Pin A0
 *
 * Pin 10 --> LED 1 --> Widerstand 1 --> GND 
 * Pin 11 --> LED 2 --> Widerstand 2 --> GND 
 * Pin 12 --> LED 3 --> Widerstand 3 --> GND 
 * Pin 13 --> LED 4 --> Widerstand 4 --> GND 
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
 */

constexpr int pin_input  = A0;
constexpr int pin_leds[] = {10, 11, 12, 13};
constexpr int n_leds     = sizeof(pin_leds) / sizeof(int);
constexpr int bucket_size = 1024 / n_leds;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(pin_input, INPUT);

  for (int i = 0; i < n_leds; i++) {
    pinMode(pin_leds[i], OUTPUT);
  }
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  static int prev_value = -1;
  int value = analogRead(pin_input);

  if (abs(value - prev_value) > 5) {
    prev_value = value;
    int min_value = 5;

    for (int i = 0; i < n_leds; i++) {
      digitalWrite(pin_leds[i], value >= min_value);
      min_value += bucket_size;
    }
  }
}
