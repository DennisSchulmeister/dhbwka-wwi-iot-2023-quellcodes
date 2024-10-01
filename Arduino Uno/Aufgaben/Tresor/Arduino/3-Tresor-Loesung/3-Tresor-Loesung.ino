/**
 * Tresorschloss
 * =============
 *
 * In dieser Aufgabe wollen wir ein Drehschloss für einen Tresor bauen, wie im folgenden
 * Video anschaulich gezeigt: https://www.youtube.com/watch?v=-DJ6CEA0i9I
 *
 * Als Drehregler verwenden wir einen "Rotary Encoder" (deutsch: Drehimpulsgeber). Äußerlich sieht
 * er wie ein Potentiometer aus, basiert jedoch nicht auf einem Spannungsteiler. Stattdessen besitzt
 * der Regler eine Rasterung, die dafür sorgt, dass bei jeder Bewegung zwei Digitalimpulse generiert
 * werden, mit denen die Drehrichtung erkannt werden kann:
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
 * Zum Entsperren des Tresors muss durch entsprechende Drehbewegungen ein Geheimcode eingegeben werden:
 * Zwei mal rechts, vier mal links, … Die erkannten Drehbewegungen und der dadurch eingegebene Code werden
 * über die serielle Verbindung protokolliert. Zwei LEDs signalisieren, ob der Tresor verschlossen oder
 * entsperrt ist.
 *
 * Wenn der Drehregler gedrückt wird, passiert folgendes: Ist der Tresor verschlossen, wird der Code
 * geprüft und der Tresor ggf. geöffnet. Ist der Tresor offen, wird er wieder verschlossen. Eine
 * Drehbewegung des Reglers verschließt den Tresor immer.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Die Schaltung können Sie anhand den Konsten unten ableiten. Es ist nicht schwer, da der Drehregler
 * direkt und die LEDs mit je einem Vorwiderstand verbunden werden können.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://sensorkit.joy-it.net/de/sensors/ky-040
 * https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino
 * https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
 * https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
 */

constexpr bool serial_output = true;

constexpr int PIN_LED_LOCKED   = 7;
constexpr int PIN_LED_UNLOCKED = 6;
constexpr int PIN_ENCODER_BTN  = 5;
constexpr int PIN_ENCODER_CLK  = 4;
constexpr int PIN_ENCODER_DT   = 3;


/**
 * Klasse zum Auslesen eines Drehimpulsgebers (Rotary Encoder) mit Mitteltaster (Button).
 */
class RotaryEncoder {
  public:
    typedef uint8_t direction_t;
    static constexpr direction_t ROTATION_NONE  = 0;
    static constexpr direction_t ROTATION_LEFT  = 1;
    static constexpr direction_t ROTATION_RIGHT = 2;

    static constexpr int DEBOUNCE_MS = 100;

    /**
     * Konstruktor.
     */
    RotaryEncoder(int pin_clk, int pin_dt, int pin_btn) {
      this->pin_clk = pin_clk;
      this->pin_dt  = pin_dt;
      this->pin_btn = pin_btn;
    }

    /**
     * Hardware initialisieren. Aufzurufen in setup().
     */
    void setup() {
      pinMode(pin_clk, INPUT_PULLUP);
      pinMode(pin_dt,  INPUT_PULLUP);
      pinMode(pin_btn, INPUT_PULLUP);

      this->clk_before = digitalRead(pin_clk);
    }

    /**
     * Prüfen, ob der Regler gedreht wurde. Aufzurufen in loop().
     */
    direction_t read_encoder() {
        direction_t direction = this->ROTATION_NONE;
        int clk = digitalRead(this->pin_clk);

        if (this->clk_before != clk) {
          this->clk_before = clk;

          if (clk == LOW) {
            int dt = digitalRead(this->pin_dt);
            direction = this->ROTATION_RIGHT;
            if (dt == LOW) direction = this->ROTATION_LEFT;

            delay(this->DEBOUNCE_MS);
          }
        }

        return direction;
    }

    /**
     * Prüfen, ob der Button gedrückt und wieder losgelassen wurde.
     * Aufzurufen in loop().
     */
    bool button_pressed() {
      bool button_pressed = false;

      if (digitalRead(this->pin_btn) == LOW) {
        delay(this->DEBOUNCE_MS);
        if (digitalRead(this->pin_btn) == LOW) button_pressed = true;
      }

      if (this->button_pressed_before != button_pressed) {
        this->button_pressed_before = button_pressed;
        if (!button_pressed) return true;
      }

      return false;
    }

  private:
    int pin_clk;                                        // Pin für CLK-Signal (Clock)
    int pin_dt;                                         // Pin für DT-Signal (Direction)
    int pin_btn;                                        // Pin für Button

    int clk_before = LOW;                               // CLK-Signal beim letzten Aufruf
    bool button_pressed_before = false;                 // Button beim letzten Aufruf
};

/**
 * Klasse zur Kapselung der Tresorlogik.
 */
class Vault {
  public:
    // Länge des Geheimcodes
    static constexpr int code_length = 4;

    // Geheimcode: Positive Zahlen stehen für eine Rechtsdrehung, negative Zahlen für
    // eine Linksdrehung. Rechts und Links müssen sich immer abwechseln.
    static constexpr int secret_code[code_length] = {2, -4, 1, -2};

    /**
     * Konstruktor.
     */
    Vault(int pin_led_locked, int pin_led_unlocked) {
      this->pin_led_locked   = pin_led_locked;
      this->pin_led_unlocked = pin_led_unlocked;
    }

    /**
     * Hardware initialisieren. Aufzurufen in setup().
     */
    void setup() {
      pinMode(pin_led_locked, OUTPUT);
      pinMode(pin_led_unlocked, OUTPUT);

      this->lock(true);
    }

    /**
     * Zustand des Tresors durch die LEDs signalisieren. Aufzurufen in loop().
     */
    void write_status_LEDs() {
      digitalWrite(this->pin_led_locked, this->locked);
      digitalWrite(this->pin_led_unlocked, !this->locked);
    }

    /**
     * Bei einer Bewegung des Drehgebers, das eingegebene Codewort aktualisieren.
     * Aufzurufen in loop().
     */
    void handle_rotary_encoder(RotaryEncoder::direction_t direction) {
      // Tresor bei einer Drehung immer sperren
      if (direction == RotaryEncoder::ROTATION_NONE) return;
      if (!this->locked) this->lock(true);
      
      // Bei Änderung der Drehrichtung zur nächsten Codeziffer wechseln
      if (this->direction != direction) {
        this->direction = direction;
        this->index = (this->index + 1) % this->code_length;

        if (this->index == 0) {
          for (int i = 0; i < this->code_length; i++) {
            this->current_code[i] = 0;
          }
        }
      }

      // Aktuelle Codeziffer entsprechend der Drehrichtung hoch/runter zählen
      if (direction == RotaryEncoder::ROTATION_LEFT) {
        if (serial_output) Serial.print("Drehung Links  | ");
        this->current_code[this->index] -= 1;
      } 
      else if (direction == RotaryEncoder::ROTATION_RIGHT) {
        if (serial_output) Serial.print("Drehung Rechts | ");
        this->current_code[this->index] += 1;
      }

      if (serial_output) this->print_current_code("\n");
    }

    /**
     * Nach Drücken des Buttons den Code prüfen bzw. den Tresor wieder sperren.
     * Aufzurufen in loop().
     */
    void handle_button(bool pressed) {
      if (!pressed) return;

      if (this->locked) {
        // Versuchen, den Tresor zu öffnen
        if (serial_output) {
          Serial.print("Prüfe Code     | ");
          this->print_current_code(" | ");
        }
        
        bool locked = false;

        for (uint8_t i = 0; i < this->code_length; i++) {
          if (this->current_code[i] != this->secret_code[i]) {
            locked = true;
          }
        }

        if (serial_output) {
          Serial.print(locked ? "Code ist falsch! | " : "Code ist richtig! | ");
        }

        this->lock(locked);
      } else {
        // Tresor wieder sperren
        this->lock(true);
      }
    }

  private:
    int pin_led_locked;                                 // Pin für rote LED (Tresor gesperrt)
    int pin_led_unlocked;                               // Pin für grüne LED (Tresor entsperrt)

    int current_code[code_length] = {0, 0, 0, 0};       // Tatsächlich eingegebener Code
    int index = -1;                                     // Index der gerade eingegebenen Ziffer
    bool locked = true;                                 // Tresor gesperrt oder entsperrt

    // Aktuelle Drehrichtung
    RotaryEncoder::direction_t direction = RotaryEncoder::ROTATION_NONE;

    /**
    * Tresor sperren oder entsperren.
    */
    void lock(bool locked) {
      if (serial_output) {
        Serial.println(locked ? "Sperre Tresor" : "Entsperre Tresor");
      }
      
      this->locked    = locked;
      this->index     = -1;
      this->direction = RotaryEncoder::ROTATION_NONE;

      for (uint8_t i = 0; i < this->code_length; i++) {
        this->current_code[i] = 0;
      }
    }

    /**
    * Ausgabe des eingegebenen Codes auf der seriellen Konsole
    */
    void print_current_code(char* linebreak) {
      if (!serial_output) return;

      Serial.print("Eingegebener Code: ");

      for (uint8_t i = 0; i < this->code_length; i++) {
        Serial.print(this->current_code[i]);
        if (i < this->code_length - 1) Serial.print(", ");
      }

      Serial.print(linebreak);
    }
};


// Globale Objekte für den Drehregler und den Tresor
RotaryEncoder rotary_encoder = RotaryEncoder(PIN_ENCODER_CLK, PIN_ENCODER_DT, PIN_ENCODER_BTN);
Vault vault = Vault(PIN_LED_LOCKED, PIN_LED_UNLOCKED);


/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  delay(1000);

  if (serial_output) {
    Serial.begin(9600);
    Serial.println("DER IOT-TRESOR SAGT HALLO ...");
  }

  rotary_encoder.setup();
  vault.setup();
}


/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  vault.write_status_LEDs();
  vault.handle_rotary_encoder(rotary_encoder.read_encoder());
  vault.handle_button(rotary_encoder.button_pressed());
}
