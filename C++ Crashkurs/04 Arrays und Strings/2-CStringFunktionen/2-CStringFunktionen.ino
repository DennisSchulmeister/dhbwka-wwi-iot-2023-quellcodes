/**
 * C-String-Funktionen
 * ===================
 *
 * Immer, wenn wir eine Zeichenkette in doppelte Anführungszeichen schreiben, haben wir
 * einen C-String. Das Wissen, dass es sich dabei um einen Zeiger auf mehrere hintereinander
 * im Speicher liegende Char-Werte (ein Char-Array) handelt und dass dieses im letzten Zeichen
 * ein Null-Byte besitzt, ist daher besonders wichtig.
 *
 * Dieses Beispiel zeigt die praktische Bedeutung dessen anhand ein paar einfacher Funktionen
 * zum Umgang mit C-Strings.
 */

#include <cstring>


void setup() {
  delay(2000);
  Serial.begin(115200):
  Serial.print("\n=================================================\n");

  // Ein Speicherbereich für bis zu hundert Zeichen. Der Inhalt ist unbestimmt!
  char buffer[100];
  constexpr size_t buffer_len = sizeof(buffer) / sizeof(char);

  // Wir füllen den Speicherbereich mit lauter Null-Bytes (zweiter Parameter)
  memset(buffer, 0, buffer_len);

  // Wie viele Zeichen hat der aktuelle String im Buffer?
  printBuffer(buffer);

  // Wir kopieren einen String in den Buffer
  strncpy(buffer, "Hallo, Welt!", buffer_len);
  printBuffer(buffer);

  // Wir hängen einen weiteren String an
  strncat(buffer, " Wie geht es dir?", buffer_len);
  buffer[buffer_len - 1] = 0;

  printBuffer(buffer);

  // Sind die Strings identisch?
  if (strncmp(buffer, "Hallo, Welt! Wie geht es dir?", buffer_len) == 0) {
    Serial.println("Die Strings sind gleich.");
  } else {
    Serial.println("Die Strings sind NICHT gleich.");
  }

  // FRAGE 1: strcpy(), strcat(), ... (ohne das kleine 'n' in der Mitte), kopieren
  // immer den kompletten Quellstring in den Zielstring und brechen erst ab, wenn
  // sie ein Null-Byte im Quellstring finden. Warum ist dies mit Abstand eine der
  // häufigsten Sicherheitslücken heutiger Zeit?

  // FRAGE 2: Warum wird nach strncat() in das letzte Byte des Buffers ein Null-Byte
  // geschrieben?
}

void loop() {
}


void printBuffer(char* buffer) {
  size_t len = strlen(buffer);

  Serial.printf("INHALT DES BUFFERS...: %s\n", buffer);
  Serial.printf("LÄNGE DES STRINGS....: %u\n\n", len);
}
