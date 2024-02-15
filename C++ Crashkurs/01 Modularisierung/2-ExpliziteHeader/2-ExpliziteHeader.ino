/**
 * Quellcode-Modularisierung / Explizite Header-Dateien
 * ====================================================
 *
 * Näher am C/C++ Sprachstandard als das automatische Zusammenfügen mehrerer *.ino-Dateien
 * ist, den Quellcode in zusätzliche Header-Dateien mit der Endung *.h oder *.hpp aufzuteilen.
 * Das Prinzip ist dasselbe, nur muss man die Header-Dateien explizit mit der Anweisung
 *
 *  #include "mein-header.h"
 *
 * in die Hauptquelldatei einfügen. Dabei handelt es sich um eine Präprozessor-Anweisung,
 * die tatsächlich einfach an der Stelle ihres Auftretens den Inhalt der Datei einfügt.
 *
 * Innerhalb der Header-Datei sollte mit
 *
 *  #pragma once
 *
 * vermieden werden, dass der Header mehrfach eingebunden und dudurch mehrfach compiliert wird.
 */
#include "utils.h"

void setup() {
  sagHallo();
}

void loop() {
}