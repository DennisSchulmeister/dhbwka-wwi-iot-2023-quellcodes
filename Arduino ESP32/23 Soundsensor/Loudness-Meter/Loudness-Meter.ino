/**
 * Loudness Meter
 * ==============
 *
 * Dieses Beispiel nutzt den KY-037 Sensor (eigentlich kein Sensor, sondern ein
 * Kleinmembran-Kondensatormikrofon) des Sensor Kits, um die Umgebungslautstärke
 * zu messen. Vom Grundprinzip entspricht dies einfach einem regelmäßigen analogRead().
 * Dabei muss jedoch einiges beachtet werden:
 *
 * Hardware-Aufbau
 * ---------------
 *
 * Geschenkt. Einfach Pin A0 des Sensors mit GPIO15 des ESP32 und den Sensor
 * zusätzlich mit 3.3V und GND verbinden. Pin D0 brauchen wir nicht.
 *
 * Bipolares Signal
 * ----------------
 *
 * Schallwellen sind Schwingungen, die im zeitlichen Verlauf um einen Nullpunkt
 * pendeln. Stellen Sie sich hierfür einfach eine Sinuskurve vor, die gleichzeitig
 * auch den einfachst möglichen Ton in der Natur darstellt. Ihre Werte sind mal
 * größer Null, mal kleiner Null.
 *
 * Was wir als Lautstärke empfinden ist nicht der absolute mathematische Y-Wert.
 * Eine Schwingung (1.0; 0.8; 1.0; 1.2; ...) ist nicht lauter als zum Beispiel
 * (0.0; -0.2; 0.0; 0.2; ...). Sie sind gleichlaut, weil kleinster und größter
 * Wert in beiden Fällen genau 0.4 auseinander liegen.
 *
 * Justierung des Sensors
 * ----------------------
 *
 * Normalerweise liefert ein Mikrofon eine Wechselspannung, die um 0V herum pendelt.
 * Da wir mit den meisten Microcontrollern aber nur positive Spannungen messen
 * können, verschiebt der Sensor die erzeugte Spannung in den positiven Bereich.
 * Mit der kleinen Schraube auf dem blauen Kästchen wird diese Verschiebung eingestellt.
 *
 * Die maximale Dynamik erhalten wir, wenn das Signal um die halbe Logikspannung
 * nach oben verschoben wird. Bei 3.3V Logikpegel des ESP32 also um ca. 1.6V.
 * Eine Ruhesignal liefert dann ein leises Rauschen um 1.6V, die Spannung kann aber
 * gleichmäßig in beide Richtungen ausschlagen, sobald ein Geräusch auftritt.
 * Siehe Fotos im Quellcodeverzeichnis.
 *
 * PROBLEM: Der Sensor ist mit hoher Wahrscheinlichkeit komplett falsch justiert!
 * Die Justierung kann aber nur mit einem Osziloskop erfolgen, welches die gemessene
 * Spannung im zeitlichen Verlauf anzeigt. Ein einfacher Spannungsmesser reicht nicht,
 * da dieser nicht schnell genug reagiert. Siehe ebenfalls Fotos.
 *
 * Sampling Theorem
 * ----------------
 *
 * Die Anzahl, wie oft ein Audiosignal je Sekunde abgetastet wird, nennt man "Sample Rate".
 * Für die Funktionsweise der Lautstärkenmessung ist es wichtig, hier den richtigen Wert
 * zu finden. Hilfe bietet uns dabei das Sampling Theorem, auch nach ihren Entdeckern
 * Nyquist-Theorem oder Nyquist-Shannon-Abtasttheorem genannt:
 *
 *   > Um eine Frequenz von x Hz abtasten zu können, muss die Sample Rate mindestens
 *   > 2x Hz betragen.
 *
 * Ist die Sample Rate zu niedrig, kann die ursprüngliche Frequenz aus dem abgetasteten
 * Signal nicht mehr rekonstruiert werden. Es treten stattdessen sog. Aliasing-Frequenzen
 * auf, die an der maximal möglichen Frequenz gespiegelt sind.
 *
 * BEISPIEL: Wir wollen ein Signal mit einer Frequenz von 300 Hz abtasten. Die Sample Rate
 * beträgt aber lediglich 500 Hz. Was wir erhalten ist ein verfälschtes Signal mit einer
 * Frequenz von 50 Hz, da mit der gewählten Sample Rate maximal 250 Hz abtastbar sind.
 * Das gemessene Signal ist zusaätzlich leiser als das Original.
 *
 * Ein Problem ist, dass die unten verwendeten Methoden sehr langsam sind. Im Versuch hat
 * eine Sample Rate von 5 kHz gerade so funktioniert, 10 kHz hingegen nicht.
 *
 * Weiterhin müssen alle Frequenzen größer als (Sample Rate / 2) eigentlich durch einen
 * Tiefpassfilter aus dem Ursprungssignal entfernt werden, bevor es abgetastet wird, da
 * sonst bei realen Geräuschen immer Aliasing auftreten wird. Dies macht der Sensor
 * natürlich nicht, da er nicht für eine bestimmte Abtastrate entwickelt wurde. Für eine
 * einfache Lautstarkenmessung können wir dieses Problem allerdings ignorieren, da so auch
 * höhere Frequenzen zumindest teilweise als laute Geräusche erkannt werden können, auch
 * wenn die Lautstärke zu niedrig berechnet wird.
 *
 * Berechnung der Lautstärke
 * -------------------------
 *
 * Kommen wir nocheinmal zur Bipolarität des Audiosignals zurück. Richtig justiert messen
 * wir Werte um 2048, wenn kein Geräusch erkannt wird. Je lauter ein Geräusch wird, desto
 * mehr entfernt sich der Messerwert kurzfristig von 2048 sowohl nach oben (Maximal 4096)
 * als auch nach unten (Minimal 0). Eine einfache Strategie könnte daher sein, lediglich auf
 * den Maximalwert zu reagieren und die Ausschläge nach unten zu ignorieren. Dies liefert
 * allerdings kein sehr gutes Maß für die Lautstärke.
 *
 * Warum? Weil ein einzelner, kurzer Impuls gar nicht als laut empfunden wird, auch wenn
 * er tatsächlich sehr weit ausschlägt. Auch zwei oder drei Impulse sind nicht laut.
 * Wir "hören" lediglich ein Knistern. Laut wird es erst, wenn die Schwingung über längere
 * Zeit anhält.
 *
 * Wir müssen daher den Faktor Zeit berücksichtigen und dabei nicht die maximalen
 * Ausschläge suchen, sondern einen Mittelwert bilden. Da es sich aber um ein bipolares
 * Signal handelt, müssen wir dieses hierfür wieder auf die Null-Achse nach unten
 * verschieben, was im Code unten mit folgender Zeile geschiecht:
 *
 * ```cpp
 * float value = (analogRead(PIN) - 2048) / 2048.0;
 * ```
 *
 * Sie rechnet den gemessenen Wert so um, dass immer eine Kommazahl zwischen [-1...1]
 * raus kommt.
 *
 * Sehr gut. Doch der Mittelwert wird dann immer ca. null sein. Wir müssen daher das
 * Vorzeichen entfernen, bevor wir mit den Messwerten rechnen. In der Praxis hat sich
 * hierfür die Root-Mean-Square-Formel durchgesetzt:
 *
 *   1) Square: Quadriere die Werte, um das Vorzeichen herauszukürzen
 *   2) Mean: Bilde den Mittelwert über die quadrierten Werte
 *   3) Root: Ziehe die Wurzel vom Mittelwert, um das Endergebnis zu erhalten
 *
 * Dadurch erhalten wir nun einen vernünftigen Wert, mit dem man arbeiten kann.
 * Wichtig zu verstehen ist lediglich, dass er nicht wirklich die "Lautstärke" des
 * Signals ausdrückt (denn diese wird in der logarithmischen Skala Dezibel gemessen),
 * sondern den Energiegehalt des gemessenen Signals abschätzt. Für die Erkennung zu
 * lauter Party-Mucke reicht das aus. Seine 7.1 Atmos Surround Anlage würde man damit
 * nicht einmessen.
 *
 * Kommerzielle Lautstärkenwarner verwenden einen Zeitraum von 0,3 Sekunden aufwärts,
 * dessen Energiegehalt sie zu messen versuchen. Da dies aber sehr schnell selbst auf
 * kurze Geräusche reagiert, wurden unten 2 Sekunden gewählt, wobei die Lautstärke
 * alle 0,5 Sekunden anhand der letzten beiden Sekunden geschätzt wird. Mathematisch
 * kann man sich das als einen extrem tiefen Tiefpassfilter vorstellen. Die einzige
 * Limitierung ist hier der knappe Hauptspeicher des ESP32, der einen Puffer von mehr
 * als zwei Sekunden Länge kaum zulässt.
 */

#include <Arduino.h>
#include "freertos/task.h"
#include <math.h>
#include <cstring>


constexpr uint32_t PIN                = 34;           // Analog Input Pin
constexpr uint32_t SR                 = 5000;         // Sample Rate in Hz
constexpr uint32_t DURATION_MS        = 2000;         // Puffer-Länge in Milisekunden
constexpr uint32_t UPDATE_MS          = 500;          // Aktualisierung alle X Milisekunden

constexpr uint32_t BUFFER_LEN         = SR * DURATION_MS / 1000;
constexpr uint32_t RECORD_MMS         = 1000000 / SR;

volatile float buffer[BUFFER_LEN] = {};
volatile size_t write_index       = 0;


/**
 * Vorbereitungen
 */
void setup() {
  delay(2500);
  Serial.begin(115200);

  pinMode(PIN, INPUT);
  analogReadResolution(12);

  memset((void *) buffer, BUFFER_LEN, 0);

  xTaskCreatePinnedToCore(
    /* pcTaskCode    */  soundRecordTask,              // Auszuführende Funktion
    /* pcName        */  "Sound Recording",            // Name des Tasks
    /* uxStackDepth  */  2048,                         // Stack-Größe in Words
    /* pvParameters  */  NULL,                         // Zeiger auf Task-Parameter oder NULL
    /* uxPriority    */  0,                            // Task-Priorität: Muss hier 0 sein, damit der Watchdog Timer nicht zuschlägt!
    /* pxCreatedTask */  NULL,                         // Zeiger auf TaskHandle_t oder NULL
    /* xCoreId       */  0                             // CPU-Core
  );
}

/**
 * Main Task: Energie in den gesampelten Daten berechnen und als Zahl auf der seriellen
 * Konsole ausgeben. Anzeige mit dem Serial Plotter der Arduino IDE.
 */
void loop() {
  delay(UPDATE_MS);

  float rms = 0.0;

  for (size_t i = 0; i < BUFFER_LEN; i++) {
    float value = buffer[i];
    rms += value * value;
  }

  rms = sqrt(rms / BUFFER_LEN) * 100;

  Serial.printf("%f\n", rms);
}


/**
 * Recording Task: Aufnahme von Audiosamples in einen Ring Buffer, damit diese im Main Task
 * analysiert werden können.
 */
void soundRecordTask(void* pvParameters) {
  while (true) {
    delayMicroseconds(RECORD_MMS);

    float value = (analogRead(PIN) - 2048) / 2048.0;
    buffer[write_index] = value;

    write_index++;
    if (write_index > BUFFER_LEN) write_index = 0;
  }
}
