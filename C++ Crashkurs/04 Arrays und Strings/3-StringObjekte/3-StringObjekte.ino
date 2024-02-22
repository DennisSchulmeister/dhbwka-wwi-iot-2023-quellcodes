/**
 * String-Objekte
 * ==============
 *
 * Für einfache String-Aufgaben sollte man aus Performance-Gründen weiterhin C-Strings verwenden.
 * Muss man an einer Stelle aber viel mit den Strings "arbeiten", kann die String-Klasse der
 * Arduino-Bibliothek die Arbeit vereinfachen und Programmierfehler vermeiden helfen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/
 */

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  // Verschiedene Beispiele zur Erzeugung eines String-Objekts.
  // Ausgangslage ist jeweils ein C-String, der dem String-Objekt übergeben wird.
  String str1 = "Hallo, String!";
  printString(str1);

  String str2 = String(str1 + " Wie geht es dir?");
  printString(str2);

  // Tipfehler: Führt zum Absturz des Programms, ohne Warnung vom Compiler! :-)
  //                         ↓
  // String str2 = String(str2 + " Wie geht es dir?");

  str2.concat(" Danke gut.");
  printString(str2);

  if (str1.endsWith("???")) {
    Serial.println("Ich bin verwirrt.");
  }
}

void loop() {
}


void printString(String str) {
  //                       Umwandlung in einen C-String!
  //                       ↓
  Serial.printf("%s\n", str.c_str());
}