/**
 * Einfach eine Funktion, die in eine eigene Quelldatei ausgelagert wurde.
 */
void sagHallo() {
  Serial.begin(115200);
  Serial.println("Hallo aus der Datei Hilfsfunktionen.ino");
}