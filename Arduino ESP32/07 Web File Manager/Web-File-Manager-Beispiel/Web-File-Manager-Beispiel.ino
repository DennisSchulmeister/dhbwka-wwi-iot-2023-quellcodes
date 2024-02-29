/**
 * Web File Manager
 * ================
 *
 * Im vorherigen Beispiel haben wir gesehen, wie im internen Flash-Speicher des ESP32 ein
 * FAT-Dateisystem angelegt und benutzt werden kann. Doch wie kommen hier die Dateien hinein,
 * wenn dort beispielsweise statische HTML/CSS/JS-Inhalte für einen Webserver landen sollen,
 * die nicht vom ESP32 selbst erzeugt werden?
 *
 * Für die Arduino IDE 1.x gibt es ein Plugin, das beim Upload der Firmware automatisch auch
 * den Inhalt des Verzeichnisses `data` in das FAT-Dateisystem hochlädt. Bisher hat aber noch
 * niemand dieses Plugin für die Arduino IDE 2.x angepasst. Findige Entwickler haben jedoch
 * Programme geschrieben, die auf dem ESP32 selblst laufen können und dort zum Beispiel einen
 * FTP-Server oder eine Webanwendung zum Verwalten der Dateien bieten.
 *
 * Um genau so ein Projekt handelt es sich hier. Durch Aufspielen dieses Quellcodes auf dem
 * ESP32 wird die vorherige Programmierung temporär ersetzt, um stattdessen den ESP32 im
 * Webbrowser aufrufen und die Inhalte des FAT-Dateisystems pflegen zu können. Danach kann
 * wieder die alte Firmware aufgespielt werden.
 *
 * Benötigte Bibliotheken
 * ----------------------
 *
 * Damit das Beispiel funktioniert, müssen im Library Manager der Arduino IDE folgende Bibliotheken
 * installiert werden:
 *
 *  - ESP32 File Manager for Generation Klick ESPFMfGK
 *  - CRC32
 *
 * Danach kann der Quellcode gebaut und hochgeladen werden. Das Programm fragt beim Start nach
 * den WLAN-Zugangsdaten in der seriellen Konsole, so dass diese nicht im Quellcode hinterlegt
 * werden müssen.
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * Nicht wirklich vorhanden. Am ehesten noch das Git-Repository von ESPFMfGK:
 *
 * https://github.com/holgerlembke/ESPFMfGK/tree/main
 * https://github.com/holgerlembke/ESPFMfGK/blob/main/examples/simple/simple.ino
 */

#include <ESPFMfGK.h>
#include <FS.h>
#include <FFat.h>

#include "wifi-wizard.h"

ESPFMfGK filemgr(8080);

void setup() {
  Serial.begin(115200);
  delay(1000);

  wifi_wizard();

  filemgr.WebPageTitle = "File Manager";
  filemgr.BackgroundColor = "white";
  filemgr.textareaCharset = "accept-charset=\"utf-8\"";
  
  if (FFat.begin(true)) {
    if (!filemgr.AddFS(FFat, "Flash/FFat", false)) {
      Serial.println(F("FAT-Dateisystem konnte nicht hinzugefügt werden!"));
    }
  } else {
    Serial.println(F("FAT-Filesystem nicht gefunden!"));
  }

  if (filemgr.begin()) {
    Serial.print(F("Dateimanager bereit unter http://"));
    Serial.print(WiFi.localIP());
    Serial.println(F(":8080/"));
  } else {
    Serial.println(F("Dateimanager konnte nicht gestartet werden!"));
  }
}

void loop() {
  filemgr.handleClient();
}
