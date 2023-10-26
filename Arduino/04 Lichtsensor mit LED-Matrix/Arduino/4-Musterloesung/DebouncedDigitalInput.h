#include "Arduino.h"

/**
 * Interne Hilfsklasse zum Auslesen eines digitalen Eingangssignals. Implementiert ein Deboncing,
 * so dass der ausgelesene Wert sich erst ändert, wenn das am GPIO-Pin anliegende Signal eine
 * gewisse Mindestzeitdauer stabil ist (sich nicht mehr verändert hat), was insbesondere für
 * mechanische Sensoren (z.B. Buttons) wichtig ist.
 *
 * Durch die Kapeslung in einer Klasse wird es möglich, im Hauptprogramm einfach je Bauteil ein
 * Objekt zu erzeugen und abzufragen. Wahlweise kann ein interner Pull-Up-Widerstand aktiviert
 * werden, um das Signal vor Einstreuungen zu schützen. Wichtig ist, dass sich dadurch allerdings
 * die Logik ändert, da das Bauteil nun gegen Ground schalten muss. In diesem Fall lesen wir ein
 * true/HIGH, wenn das Bauteil *nicht* schaltet und ein false/LOW, wenn es schaltet. Ohne Pull-Up
 * ist es genau andersrum.
 *
 * Verwendung:
 *
 *   1) Im Rahnprogramm ein neues Objekt erzeugen
 *   2) In der setup()-Funktion die Methode setup() des Objekts aufrufen
 *   3) In der loop()-Funktion die Methode digitalRead() zum Abfragen des Buttons aufrufen
 */
class DebouncedDigitalInput {
  public:
    /**
     * Konstruktor.
     *
     * Parameter:
     *   - input_pin: Nummer des GPIO-Pins
     *   - internal_pullup: Internen Pull-Up-Widerstand aktivieren
     *   - debounce_ms: Zeitdauer, die das Signal stabil bleiben muss
     */
    DebouncedDigitalInput(int input_pin, bool internal_pullup, unsigned long debounce_ms) {
      this->_input_pin        = input_pin;
      this->_internal_pullup  = internal_pullup;
      this->_debounce_ms      = debounce_ms;
      this->_last_change_time = millis();
      this->_previous_value   = false;
      this->_debounced_value  = false;
    }

    /**
     * Initialisierung der Hardware. Muss in der setup()-Funktion aufgerufen werden.
     */
    void setup() {
      int mode = INPUT;
      if (this->_internal_pullup) mode = INPUT_PULLUP;

      ::pinMode(this->_input_pin, mode);

      this->_previous_value  = ::digitalRead(this->_input_pin);
      this->_debounced_value = this->_debounced_value;
    }

    /**
     * Abfragen des Eingangssignals. Muss in der loop()-Funktion bei jedem Durchlauf aufgerufen werden.
     * ACHTUNG: Wenn diese Methode nicht regelmäßig aufgerufen wird, funktioniert das Debouncing nicht.
     */
    bool digitalRead() {
      unsigned long current_time = millis();
      bool current_value = ::digitalRead(this->_input_pin);

      if (current_value != this->_previous_value) {
        this->_previous_value   = current_value;
        this->_last_change_time = current_time;
      } else if ((current_time - this->_last_change_time) > this->_debounce_ms) {
        this->_debounced_value = current_value;
      }

      return this->_debounced_value;
    }

  private:
    int _input_pin;
    bool _internal_pullup;
    unsigned long _debounce_ms;
    unsigned long _last_change_time;
    bool _previous_value;
    bool _debounced_value;
};