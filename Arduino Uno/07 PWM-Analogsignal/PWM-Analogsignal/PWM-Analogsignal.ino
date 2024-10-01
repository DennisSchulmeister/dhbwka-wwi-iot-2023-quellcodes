/**
 * PWM-Analogsignal
 * ================
 *
 * Versuchsprogramm zur Generierung eines Analogsignals auf den PWM-Pins des
 * Arduino Uno mit der Funktion analogWrite(). Leider besitzt der Arudino Uno
 * (bzw. der darauf verbaute Atmega328P) keine echten Analogausgänge (DAC),
 * obwohl Analogeingänge (ADC) vorhanden sind. analogWrite() steuert auf den
 * unterstützten Pins daher stattdessen die Pulsweite eines PWM-Signals, was
 * für viele Anwendungsfälle (LED dimmen, Motorgeschwindigkeit regeln, ...)
 * denselben Effekt hat. Es ist aber eben kein echtes Analogsignal, da bei
 * PWM ja durch schnelles Ein- und Ausschalten nur die zeitlich gemittelte
 * Spannung reduziert wird.
 *
 * Dieser Sketch versucht mit einem kleinen Hardwareaufbau dennoch ein
 * Analogsignal zu generieren. Denn ein PWM-Signal mit sehr hoher Frequenz
 * (ca. 10x so hoch wie die analoge Zielfrequenz) kann durch einen einfachen
 * Tiefpassfilter "integriert" und somit in die gewünschte Analogschwingung
 * umgewandelt werden. Auf dem Raspberry Pi wird somit das am Kopfhörerausgang
 * anliegende Audiosignal erzeugt, obwohl der Pi gar keine Analogwandler besitzt.
 *
 * Grundsätzlich funktioniert dasselbe Prinzip auch mit den PWM-Pins des
 * Arduino Uno. Jedoch ist die erzielbare Auflösung sehr beschränkt, da zum
 * einen die PWM-Frequenz mit ca. 500 Hz sehr gering ist und die Pulsweite
 * (die anhand der gewünschten Schwingungsform moduliert werden muss) nur
 * in 255 Stufen variiert werden kann. Dies entspricht einem 8-Bit digitalen
 * Analogsignal mit sehr geringer Sample Rate. Wie der Sketch daher zeigt
 * (zumindest wenn man mit den Parametern spielt und sich das Ergebnis auf
 * einem Oszilloskop anschaut), wäre der Anschluss eines seriell angebundenen
 * DA-Wandlers daher eine deutlich bessere Lösung. :-)
 *
 * Technische Randnotiz: Die Klasse Oscilator in der Datei lib/Oscilator.h
 * zeigt, wie ein einfacher Wavetable-Oszilator zur Generierung unterschiedlicher
 * Schwingungsformen selbst implementiert werden kann.
 *
 * Hardwareaufbau
 * --------------
 *
 * Zur Umwandlung der PWM-Signale in Analogsignale kann ein einfacher RC-Filter
 * aufgebaut werden. [3] beinhaltet einen Online-Rechner hierfür. Beispielsweise
 * kann mit einem Widerstand von 10 Ohm und einem Kondensator von 220 µF ein
 * Filter mit 33 Hz Trennfrequenz wie folgt aufgebaut werden:
 *
 *   [Adruino-Pin]-----[Widerstand 10 Ohm]--.---Analogsignal
 *                                          |
 *                                  [Kondensator 220 µF]
 *                                          |
 *                                        (GND)
 *
 * Die Filterfrequenz erscheint zunächst sehr gering, verglichen mit der erzeugten
 * 100 Hz Schwingungsform. Allerdings handelt es sich bei dem realisierten Filter
 * um einen Filter 1. Ordnung, der alle Frequenzanteil oberhalb der Trennfrequenz
 * mit 3dB je Oktave absenkt. Dies entspricht der halben Signalstärke (6 dB) je
 * Vierfachem (zwei Oktaven) der Trennfrequenz, wodurch die gewünschten 100 Hz auf
 * dem Oszilloskop mit geringerer Spannung angezeigt werden, aber eben nicht komplett
 * herausgefiltert werden. Im praktischen Versuchsaufbau zeigten sich diese Werte
 * als geeignet, um das Prinzip PWM-Signal-zu-Analog zu veranschaulichen.
 *
 * Um die Signale mit einem Oszilloskop zu betrachten, empfehlen sich folgende
 * Einstellungen am Oszilloskop. Der [AUTO]-Knopf sollte in vielen Fällen passende,
 * ähnliche Werte finden:
 *
 *  - Sec/Div (Zeitauflösung entlang X-Achse): 800µs - 8.00ms
 *  - Horiz (Verschiebung auf der X-Achse): 0.000s
 *  - Volts/Div (Wertauflösung entlang Y-Achse): 2V
 *  - Persistence (Nachleuchten des Signals): Auto
 *
 * Manche Oszilloskope können die Signale auch zeitlich mitteln (Acquire Mode -> Average).
 * Bei 64 Samples lässt sich somit schön der Mittelwert des PWM-Signals zeigen, der
 * fast wie das durch den Filter erzeugte Analogsignal aussieht (da die mathematische
 * Mittelwertbildung tatsächlich einem Tiefpassfilter entspricht).
 *
 * Auf dem Oszilloskop sieht man allerdings auch die Einschränkungen der Funktion
 * analogWrite() mit PWM auf dem Arduino. Das Signale ist extrem verzerrt und
 * verrauscht. Auf einem Kopfhörer ausgegeben ist das auf Masse anliegende Eigenrauschen
 * des Arduino (bemerkbar durch Zierpen und Piepsen) lauter als das eigentliche
 * Zielsignal. Zum Vergleich kann unten das #define SERIAL_OUTPUT einkommentiert
 * werden, um die theoretische Schwingungsform im Serial Plotter der Arudino IDE zu sehen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * [1] https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
 * [2] https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
 * [3] http://www.learningaboutelectronics.com/Articles/Low-pass-filter-calculator.php
 */
#include "Oscilator.h"

// #define SERIAL_OUTPUT

const int PIN_SIN = 11;
Oscilator oscilator;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(PIN_SIN, OUTPUT);

  #ifdef SERIAL_OUTPUT
  Serial.begin(9600);
  #endif
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  int sin_value = oscilator.sin(50);
  analogWrite(PIN_SIN, sin_value);

  #ifdef SERIAL_OUTPUT
  Serial.println(sin_value);
  #endif
}
