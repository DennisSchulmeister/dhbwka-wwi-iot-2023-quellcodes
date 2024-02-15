/**
 * Quellcode-Modularisierung / Kompiliereinheiten
 * ==============================================
 *
 * Dies ist fast dasselbe wie im Beispiel davor. Auch hier wurde ein Teil des Quellcodes
 * in eine *.h Header-Datei ausgelagert. Diese definiert aber nur die enthaltene Funktion.
 * Die Implementierung befindet sich stattdessen in einer *.cpp-Datei da als eigenständige
 * Einheit kompiliert wird.
 *
 * WICHTIG: In die *.cpp-Dateien muss die Zeile
 *
 *   #include <Arduino.h>
 *
 * eingefügt werden, damit die eingebauten Arudino-Funktionen dort genutzt werden können.
 * Bei den *.ino-Dateien wird diese Zeile im Hintergrund automatisch eingefügt, wenn sie fehlt.
 *
 * Das Vorgehen ist dabei so, dass zunächst jede *.ino und jede *.cpp-Datei vom Compiler
 * nur zu einem Objektmodul kompiliert wird. Die Objektmodule werden daraufhin vom Linker
 * zu einer tatsächlich ausführbaren Datei zusammengesetzt. Dies klingt erst mal so, als
 * würde es die Sache unnötig verkomplizieren. Durch dieses Vorgehen kann man tatsächlich
 * aber viel Zeit sparen, da bei einer Quellcodeänderung meist nur ein einzelnes
 * Objektmodul neu kompiliert werden muss.
 */
#include "utils.h"

void setup() {
  sagHallo();
}

void loop() {
}