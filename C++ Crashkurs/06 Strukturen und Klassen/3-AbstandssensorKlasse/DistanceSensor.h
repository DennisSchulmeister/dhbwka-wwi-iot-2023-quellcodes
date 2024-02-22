#include <Arduino.h>


class DistanceSensor {
  public:

    // Konstruktor
    DistanceSensor(int trigger_pin, int echo_pin) {
      this->trigger_pin = trigger_pin;
      this->echo_pin = echo_pin;

      pinMode(this->trigger_pin, OUTPUT);
      pinMode(this->echo_pin, INPUT);
    }

    // Entfernung messen
    long measure_distance_cm() {
      digitalWrite(this->trigger_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(this->trigger_pin, LOW);

      auto duration = pulseIn(this->echo_pin, HIGH);
      return duration / 58.2;
    }

  private:
    int trigger_pin;
    int echo_pin;
};