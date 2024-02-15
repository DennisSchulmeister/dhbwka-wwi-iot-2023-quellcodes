// Die nachfolgende Zeile sorgt dafür, dass der Präprozessor diese Header-Datei
// ignoriert, wenn sie schon an anderer Stelle in den Hauptquellcode eingefügt wurde.
// Das ist vor allem dann wichtig, innerhalb der Header-Dateien ebenfalls mit #include
// Header-Dateien eingebunden werden.
#pragma once

/**
 * Einfach eine Funktion, die in eine eigene Quelldatei ausgelagert wurde.
 */
void sagHallo() {
  Serial.begin(115200);
  Serial.println("Hallo aus der Datei utils.h");
}