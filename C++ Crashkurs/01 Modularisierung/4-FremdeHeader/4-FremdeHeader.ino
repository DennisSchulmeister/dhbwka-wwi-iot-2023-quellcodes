/**
 * Quellcode-Modularisierung / Fremde Header
 * =========================================
 *
 * Natürlich ist die #include-Anweisung nicht nur dafür gut, den eigenen Quellcode in
 * mehrere Dateien aufzuteilen. Sie dient auch dazu, fremde Quellcodes zu importieren,
 * die zuvor als Quellcode-Bibliothek auf dem eigenen Rechner installiert wurden.
 *
 * In diesem Beispiel binden wir die Datei "esp_log.h" aus dem ESP32 IDF ein, um die
 * darin enthaltenen Funktionen zum Erzeugen von Logausgaben nutzen zu können.
 *
 * Wann verwendet man `#include <datei.h>` und wann `#include "datei.h"`?
 * ----------------------------------------------------------------------
 *
 * Bei genauerer Betrachtung haben Sie vielleicht festgestellt, dass bei manchen
 * #include-Anweisungen der Name der fremden Quelldatei in Anführungszeichen und mal
 * in spitzen Klammern steht. Dies hat folgende Bedeutung:
 *
 *    * In Anführungszeichen: Die Datei ist Bestandteil des aktuellen Projekts. Der
 *      bei #include eingegebene Pfad ist daher eine relative Pfadangabe innerhalb
 *      des eigenen Projektverzeichnisses.
 *
 *    * In spitzen Klammern: Die Datei ist Teil einer externen Bilbliothek und als solche
 *      daher innerhalb des Projekts nicht auffindbar. Sie wird im "Systemsuchpfad" gesucht.
 *
 * Wichtiger Hinweis
 * -----------------
 *
 * ACHTUNG: Damit Sie die Logausgaben auf der seriellen Konsole tatsächlich sehen, muss
 * in der Arduino IDE unter "Tools -> Core Debug Level" der Wert auf Information oder
 * kritischer gesetzt werden.
 */

#include <esp_log.h>

#define TAG "4 Fremde Header"

void setup() {
  ESP_LOGI(TAG, "setup()-Funktion wurde aufgerufen.");
}

void loop() {
  ESP_LOGI(TAG, "Ein weiterer loop()-Durchlauf beginnt.");
  delay(2000);
}
