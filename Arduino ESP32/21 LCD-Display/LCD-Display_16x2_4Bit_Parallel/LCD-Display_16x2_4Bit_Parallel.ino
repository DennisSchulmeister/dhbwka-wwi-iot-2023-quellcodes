/**
 * LCD-Display (via 4-Bit Parallel-Port)
 * =====================================
 *
 * Dieses Beispiel zeigt, zwei ein klassisches, textbasiertes LCD-Dispay mit 16x2 Zeichen über
 * den ESP32 angesteuert werden kann. Angesteuert wird das Display ebenfalls klassisch über die
 * parallele Schittstelle des im Display verbauten HD44780-kompatiblen Controller-Chips. Auf
 * diese Weise kann sehr preisgünstig eine einfache Textausgabe für Statusmeldungen, Menüeingaben
 * usw. realisiert werden.
 *
 * Teurere Displays bieten mehr Zeichen, mehr grafische Möglichkeiten oder eine einfachere
 * Ansteuerung mit weniger Pins beispielsweise über I²C oder SPI. Die hier gezeigte Variante
 * entspricht jedoch dem "Industristandard", wie er seit den 1980er-Jahren in unzähligen
 * Geräten verbaut wird.
 *
 * Wir beschränken uns in dem Beispiel auf eine einfache Textausgabe mit dem eingebauten Zeichenasatz
 * des Display-Controllers, auch wenn dieser oft keine Umlaute enthält. Wer sich darüf interessiert,
 * findet unter folgendem Link eine ausführlichere Anleitung, die auch die Verwendung eigener
 * Schriftzeichen demonstriert: https://www.makerguides.com/interfacing-esp32-and-16x2-lcd-parallel-data-without-i2c/
 *
 * Viele weitere Beispiele: https://github.com/arduino-libraries/LiquidCrystal/tree/master/examples
 *
 * Hardwareaufbau
 * ==============
 *
 * Zunächst muss das LCD-Display an zwei Stellen mit einer 5V Stromquelle verbunden werden.
 * VSS und VDD bilden die Stromversorgung für das Display selbst. A und K sind Anode und
 * Kathode der Hintergrundbeleuchtung.
 *
 *    LCD-Display    ESP32
 *    -----------    -----
 *    VSS            GND
 *    VDD            VIN (5V)
 *    A              VIN (5V)
 *    K              GND
 *    V0             GND
 *
 * V0 dient der Kontrastregelung des Displays. Hier kann ein Potentiometer angeschlossen werden,
 * der als Spannungsteiler eine variable Spannung zwischen 0...5V generiert. Im praktischen Versuch
 * hat sich aber gezeigt, dass es oft ausreicht, den Pin einfach mit Ground zu verbinden.
 *
 * VORSICHT: Bleibt V0 unverbunden, sieht man auf dem Display in der Regel nichts.
 *
 * Zur Ansteuerung des Displays gibt es acht Datenleitungen, von denen wir nur vier benutzen,
 * um die Verkabelung zu vereinfachen und drei Steuereingänge.
 *
 *    LCD-Display    ESP32
 *    -----------    -----
 *    RS             GPIO 22
 *    RW             GND
 *    E              GPIO 23
 *    D4             GPIO 5
 *    D5             GPIO 18
 *    D6             GPIO 19
 *    D7             GPIO 21
 *
 * Die komische Reihenfolge der ESP32-GPIOs ergibt sich aus deren Anordnung auf der Platine.
 * Ihre physische Anordnung entspricht von links nach rechts genau den Anschlüssen des Displays.
 * Die Ein- und Ausgänge haben folgende Bedeutung:
 *
 *    * RS: 0 = Instruction Register, 1 = Data Register
 *    * RW: 0 = Write, 1 = Read
 *    * E:  1 = LCD-Controller aktivieren
 *    * D4-D7: Parallele Datenleitungen
 *
 * RS, RW und E sind aus Sicht des ESP32 reine Ausgänge. D4-D7 sind Ein- und Ausgänge, je nachdem
 * ob Daten an den LCD-Controller geschickt oder von ihm abgerufen werden.
 *
 * Warum so viele Pins? Der HD44780-Baustein wurde in den 1980er-Jahren entwickelt, als Microcontroller
 * kaum verbreitet waren. Seine Hardwareschnittstelle ist so gestaltet, dass der Baustein direkt mit dem
 * Systembus eines für damalige Zeiten typischen Computersystems verbunden werden kann. RW und D0...D7
 * sind in diesem direkt verfügbar (außer bei 4-Bit-Systemen, bei denen D4...D7 fehlen). Dann bleiben
 * nur noch RS und E, die über eine Dekodierlogik mit den Adressleitungen verbunden werden. Dadurch "sieht"
 * die CPU das Display an einer bestimmten Speicheradresse und muss lediglich von dieser Adresse Daten
 * lesen oder schreiben, um das Display anzusteuern.
 *
 * Benötigte Bibliotheken
 * ======================
 *
 * Folgende Bibliothek muss in der Aduino IDE mit dem Library Manager installiert werden,
 * um das LCD-Display ansteuern zu können:
 *
 * LiquidCrystal
 *
 * VORSICHT: Es gibt mehrere Bibliotheken mit dem Namen LiquidCrytal. Für dieses Beispiel
 * verwenden Sie bitte die Bibliothek mit der oben gezeigten Schreibweise. Sie sollte keine
 * Abhängigkeiten besitzen.
 */

#include <LiquidCrystal.h>

constexpr uint8_t PIN_RS = 22;
constexpr uint8_t PIN_E  = 23;
constexpr uint8_t PIN_D4 = 5;
constexpr uint8_t PIN_D5 = 18;
constexpr uint8_t PIN_D6 = 19;
constexpr uint8_t PIN_D7 = 21;

/**
 * VORSICHT: Das Objekt darf hier noch nicht erzeugt werden. Sonst hängt sich der ESP32
 * beim Starten auf und kommt nie in die setup()-Funktion. Deshalb wird hier anstelle
 * einer Objektreferenz ein Zeiger auf ein Objekt definiert.
 */
LiquidCrystal *lcd;


// Name für die Log-Ausgaben
static const char* TAG = "LCD-Beispiel";


void setup() {
  ESP_LOGD(TAG, "Initialisiere LCD-Display");
  lcd = new LiquidCrystal{PIN_RS, PIN_E, PIN_D4, PIN_D5, PIN_D6, PIN_D7};   // Hier wird nun das Objekt erzeugt

  lcd->begin(16, 2);      // Spalten, Zeilen
  lcd->display();

  ESP_LOGD(TAG, "Schreibe Text in erste LCD-Zeile");
  lcd->setCursor(0, 0);   // Spalte, Zeile bei Null anfangend
  lcd->print("Hallo ESP32");
}


void loop() {
  delay(1000);

  ESP_LOGD(TAG, "Aktualisiere Zeit in der zweiten LCD-Zeile");
  lcd->setCursor(0, 1);
  lcd->print(millis() / 1000);
}
