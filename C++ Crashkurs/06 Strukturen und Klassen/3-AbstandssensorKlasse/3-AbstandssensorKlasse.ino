/**
 * Abstandssensor, Version 3: Klasse
 * =================================
 *
 * Die finale Version unseres Beispiels. Hier wurden Struktur und Funktionen von zuvor zu
 * einer wiederverwendbaren, vollständig gekappselten Klasse zusammengefasst.
 *
 * TIPP: Strukturen sind in C++ nichts anderes als Klassen mit nur public-Attributen.
 * Theoretisch hätten wir auch bei der Struktur bleiben können.
 */

// Die Klasse befindet sich in einer wiederverwendbaren Header-Datei
#include "DistanceSensor.h"

DistanceSensor sensor(23, 22);


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");
}


void loop() {
  delay(1000);

  int distance_cm = sensor.measure_distance_cm();
  Serial.printf("Entfernung: %u cm\n", distance_cm);
}
