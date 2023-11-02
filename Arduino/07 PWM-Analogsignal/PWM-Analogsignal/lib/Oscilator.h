/**
 * Vorberechnete Sinus-Tabelle mit 256 Werten von 0 bis 2*pi, skaliert auf Integer
 * im Zahlenbereich 0 bis 255. Generiert in LibreOffice-Calc mit folgender Funktion:
 *
 * ROUND(((SIN((2 * $I$1 * A1) / $I$2) +1) / 2) * $I$3; 0)
 *
 *  - Spalte A enthält hierfür die Indizies von 0 bis 255.
 *  - Feld I1 die Formal ACOS(-1) zur Berechnung von Pi.
 *  - Feld I2 die Anzal der Tabelleneinträge: 64
 *  - Feöd I3 den gewünschten Maximalwert: 255
 *  - Spalte B die obige Formel zur Berechnung der Werte.
 */
const PROGMEM int SINE_TABLE[] = {
  128, 140, 152, 165, 176, 188, 198, 208, 218, 226, 234, 240, 245, 250, 253, 254,
  255, 254, 253, 250, 245, 240, 234, 226, 218, 208, 198, 188, 176, 165, 152, 140,
  128, 115, 103,  90,  79,  67,  57,  47,  37,  29,  21,  15,  10,   5,   2,   1,
    0,   1,   2,   5,  10,  15,  21,  29,  37,  47,  57,  67,  79,  90, 103, 115,
};

/**
 * Klasse zur Berechnung typischer Analogsignale, die mit analogWrite() auf einem
 * Pin ausgegeben werden können. Entsprechend der Einschränkungen des Arduinos
 * werden die Werte als vorzeichenlose 8-Bit Ganzzahlen berechnet, was zwar keine
 * sehr hohe Auflösung garantiert, für diesen Versuch aber ausreicht.
 *
 * Zur Berechnung einer Schwingungsform muss ein Objekt dieser Klasse erzeugt und
 * dann regelmäßig eine der enthaltenen Funktionen aufgerufen werden. Die Funktion
 * erwartet als Parameter die Frequenz in Herz (Zyklen je Sekunde). Anhand der
 * vergangenen Zeit seit dem letzten Aufruf wird dann der nächste Wert berechnet.
 *
 * Wenn mehrere Signaltypen mit derselben Frequenz benötigt wird, kann hierfür eine
 * einzige Objektinstanz verwendet werden. Sollen die Signale jedoch unterschiedliche
 * Frequenzen besitzen, muss jeweils ein eigenes Objekt erzeugt werden.
 */
class Oscilator {
  public:
    /**
     * Phasor: Steigt in einem Durchlauf linear von 0 bis 255 auf und beginnt dann beim
     * nächsten Durchlauf wieder bei 0.
     */
    int phasor(float frequency) {
      this->update_phase(frequency);
      return this->_phase;
    }

    /**
     * Rechteckwelle, die periodisch zwischen 0 und 255 wechselt. Standardmäßig wird eine
     * Pulsbreite von 50% angeneommen, so dass das Signal im ersten Halbzyklus an und im
     * zweiten Halbzyklus aus ist. Die Pulsbreite kann aber auch von 0 bis 255 variiert werden. 
     */
    int pwm(float frequency, int width=127) {
      this->update_phase(frequency);
      return this->_phase <= width ? MAX_VALUE : 0;
    }

    /**
     * Sägezahnwelle: Steigt erst linear von 0 bis 255 auf und dann wieder zu 0 hinab.
     * Standardmäßig liegt der Scheitelpunkt genau in der Mitte eines Zyklus, so das Auf-
     * und Abstieg gleichlang sind. Der Wert kann aber auch von 0 bis 255 varriert werden.
     */
    int saw(float frequency, int width=127) {
      this->update_phase(frequency);

      if (this->_phase <= width) {
        int ramp_up = width;
        return (MAX_VALUE / ramp_up) * this->_phase;
      } else {
        int ramp_down = 255 - width;
        return (MAX_VALUE / ramp_down) * (this->_phase - width);
      }
    }

    /**
     * Sinuswelle mit Hilfe der obenstehenden Lookup Table.
     */
    int sin(float frequency) {
      this->update_phase(frequency);
      return pgm_read_byte_near(SINE_TABLE + this->_phase);
    }
  
  private:
    const int TABLE_SIZE = 64;
    const int MAX_VALUE = 255;

    int _phase = 0;
    unsigned long _micros = micros();

    /**
     * Hilfsmethode zur Aktualisierung der obigen Variablen _phase und _micros. Zählt _phase
     * entsprechend der übergebenen Frequenz von 0 bis 255 hoch und fängt dann wieder von
     * Vorne an. Hierfür wird intern die micros()-Funktion von Arduino genutzt, um die Zeit
     * seit dem letzten Aufruf zu ermitteln. Je nach Arduino-Modell beträgt die Genauigkeit
     * daher 4 oder 8 Microsekunden.
     */
    void update_phase(float frequency) {
      float wait_time = (1000000 / frequency) / TABLE_SIZE;
      unsigned long micros1 = this->_micros;
      unsigned long micros2 = micros();

      if ((micros2 < micros1) || (micros2 - micros1 >= wait_time)) {
        this->_phase++;
        if (this->_phase >= TABLE_SIZE) this->_phase = 0;

        this->_micros = micros2;
      }
    }
};