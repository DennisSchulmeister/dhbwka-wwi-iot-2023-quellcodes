/**
 * FAT-Dateisystem
 * ===============
 *
 * Ein interessantes Feature, das den ESP32 von anderen Arduino-Boards abhebt, ist der
 * eingebaute Flash-Speicher, der auf derselben Technologie wie bei einem USB-Stick oder
 * oder einer (sehr sehr kleinen) SSD basiert. Dieser kann für allerlei nützliche Dinge
 * wie Over-The-Air-Updates oder zur Ablage von Dateien genutzt werden. Diese Funktion
 * können wir später nutzen, um statische Dateien für einen auf dem ESP32 laufenden
 * Webserver hochladen zu können (HTML, CSS, JS, ...), ohne diese in den C/C++ Quellcode
 * aufnehmen zu müssen.
 *
 * Dies ist ein Minimalbeispiel für die Verwaltung eines einfachen FAT-Dateisystems
 * (kompatibel zu Microsoft Windows) im internen Flash-Speicher des ESP32. Es handelt
 * sich um eine reduzierte Version des Beispiels, das sich in der Arduino IDE unter "File
 * -> Examples -> FFat -> FFat_Test" aufrufen lässt, sofern das aktuell ausgewählte Board
 * ein ESP32-Board ist.
 *
 * Arduino-Einstellungen
 * ---------------------
 *
 * Damit das Beispiel funktioniert, muss im Flash-Speicher des ESP32 ein FAT-Dateisysystem
 * für Anwendungsdaten angelegt werden. Dies lässt sich am einfachsten durch Auswahl des
 * folgenden Menüpunkts in der Arduino IDE sicherstellen: "Tools -> Partition Scheme ->
 * Default 4MB with ffat". Das "with ffat" ist hier ausschlaggebend. Danach kann das Projekt
 * gebaut und hochgeladen werden.
 *
 * Theoretisch kann man neben den im Mneü angezeigten Partitionsschemas auch sein eigenes
 * Schema definieren und als CSV-Datei im Quellcodeverzeichnis ablegen. Für unsere Zwecke
 * reicht aber die Voreinstellung.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * Die FFat und FS-Klasse scheinen keine Dokumentation zu besitzen. Die beste Dokumentation
 * sind das in der Arduino IDE aufrufbare Beispiel sowie die Header Files selbst:
 *
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/FFat/src/FFat.h
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/FS/src/FS.h
 */

#include "FS.h"
#include "FFat.h"
#include <errno.h>


// Setzen Sie diese Konstante auf true, um das Dateisystem beim nächsten Start zu formatieren
constexpr bool FORMAT_FFAT_FILESYSTEM = false;


/**
 * Arduino Setup-Funktion
 */
void setup() {
  delay(3000);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Dateisystem formatieren
  if (FORMAT_FFAT_FILESYSTEM) {
    Serial.println("Formatiere FAT-Dateisystem");
    FFat.format();
  }

  // Dateisystem einhängen
  Serial.println("Hänge FAT-Dateisystem ein");

  if (!FFat.begin()) {
    Serial.println("Hat leider nicht geklappt. :-(");
    return;
  }

  // Freien Speicherplatz abfragen
  size_t free_bytes   = FFat.freeBytes();
  size_t total_bytes  = FFat.totalBytes();
  size_t used_bytes   = FFat.usedBytes();
  float  percent_used = 100.0 * used_bytes / total_bytes;

  Serial.printf("Freier Speicher: %u Bytes von %u Bytes (%.0f%% belegt)\n", free_bytes, total_bytes, percent_used);

  // Inhalt des Stammverzeichnisses auflisten
  Serial.println("\nInhalt des Stammverzeichnisses:\n");

  File root_dir  = FFat.open("/");
  File next_file = root_dir.openNextFile();

  if (!next_file) {
    Serial.println("  -- Keine Inhalte gefunden --");
  }Serial.setDebugOutput(true);

  while (next_file) {
    const char*  file_name = next_file.name();
    const size_t file_size = next_file.size();

    if (next_file.isDirectory()) {
      Serial.printf(" - DIR....: %s\n", file_name);
    } else {
      Serial.printf(" - FILE...: %s    SIZE: %u Bytes\n", file_name, file_size);
    }

    next_file = root_dir.openNextFile();
  }

  Serial.println();

  // Verzeichnis "test" anlegen, falls noch nicht vorhanden
  if (!FFat.exists("/test")) {
    Serial.println("Lege Verzeichnis /test an");
    
    if (!FFat.mkdir("/test")) {
      Serial.println("Hat leider nicht geklappt. :-(");

      const char* error_name = strerror(errno);
      Serial.printf("Fehlercode: %s\n", error_name);
    }
  } else {
    Serial.println("Verzeichnis /test existiert bereits.");
  }

  // Datei "/test.txt" anlegen und etwas Text hinein schreiben
  Serial.println("Schreibe ein Zeilen in die Datei /test.txt");

  File test_file = FFat.open(
    /* path */    "/test.txt",
    /* mode */    FILE_WRITE,
    /* create */  true
  );

  for (int i = 1; i <= 15; i++) {
    test_file.printf("Zeile %i\n", i);
  }

  test_file.close();

  // Inhalt der Datei "/test.txt" ausgeben
  Serial.println("\nInhalt der Datei /text.txt:\n");

  test_file = FFat.open("/test.txt", FILE_READ);

  char buffer[100];
  size_t bytes_read = 0;

  do {
    bytes_read = test_file.readBytes(buffer, 100);

    for (int i = 0; i < bytes_read; i++) {
      Serial.write(buffer[i]);
    }
  } while (bytes_read > 0);

  test_file.close();
}


/**
 * Hier nicht benötigt: Endlosschleife mit der Hauptprogrammlogik
 */
void loop() {
}
