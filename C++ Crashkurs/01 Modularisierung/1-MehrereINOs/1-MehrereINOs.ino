/**
 * Quellcode-Modularisierung / Mehrere INOs
 * ========================================
 *
 * Eine Besonderheit der "Arduino Language" ist, dass jedes Projekt in einem eigenen
 * Verzeichnis liegen muss, das genau so heißt, wie die *.ino-Datei mit dem Quellcode
 * des Projekts. Will man den Quellcode nun in mehrere Dateien aufteilen, kann man in
 * diesem Verzeichnis einfach weitere *.ino-Dateien anlegen. Sie werden beim Bauen
 * der Firmware alle zu einer großen Datei zusammengefügt.
 *
 * ACHTUNG: Die *.ino-Dateien werden in alphabetischer Reihenfolge miteinander verschmolzen.
 * Eventuell müssen daher Funktionen, Strukturen oder Klassen, die in einer anderen Datei
 * implementiert sind, an manchen Stellen per Vorwärtsdeklaration bekannt gemacht werden.
 *
 */

void setup() {
  sagHallo();
}

void loop() {
}
