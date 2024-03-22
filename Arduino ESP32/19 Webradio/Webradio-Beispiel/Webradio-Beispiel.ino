/**
 * Webradio
 * ========
 *
 * Dieses Beispiel zeigt, wie der ESP32 einen Live-Audiostream aus dem Internet empfangen und
 * wiedergeben kann. Die Wiedergabe kann entweder über die eingebauten zwei DACs oder über einen
 * via I²S angebundenen Baustein erfolgen. Letzteres wird empfohlen, da die internen DACs nur
 * 8-Bit Qualität liefern.
 *
 * Hardwareaufbau
 * ==============
 *
 * Der ESP32 besitzt zwei I²S-Module in Hardware, die jeweils im Half-Duplex-Modus funktionieren
 * und somit entweder einen Stereo-Audiostream senden oder empfangen können. Für Full-Duplex
 * müssen beide Einheiten kombiniert werden. Dieses Beispiel nutzt jedoch nur die erste Einheit,
 * da lediglich ein Audiosginal wiedergegeben werden soll. Die Pins sind frei wählbar und können
 * über die Konstanten im Quellcode angepasst werden:
 *
 *   - GPIO5       -> Serial Data
 *   - GPIO18      -> Serial Clock
 *   - GPIO19      -> L/R Clock
 *   - GPIO1 (TX0) -> Master Clock
 *
 * Die internen DACs des ESP32 sind fest mit den GPIOs 25 und 26 verdrahtet. Hier kann ein kleiner
 * Kopfhöher angeschlossen werden:
 *
 *   - GPIO25 (DAC_1) -> Left Audio
 *   - GPIO26 (DAC_2) -> Right Audio
 *   - GND -> Masse
 *
 * Benötigte Bibliotheken
 * ======================
 *
 * Dieses Beispiel setzt die Bibliothek ESP32-audioI2S voraus, die unter folgender Adresse
 * heruntergeladen weren kandn: https://github.com/schreibfaul1/ESP32-audioI2S/tags
 *
 * Zur Installation laden Sie eine ZIP-Datei herunter und speichern diese ohne Entpacken
 * ab. Anschließend klicken Sie in der Arduino IDE auf "Sketch -> Include Library ->
 * Add .ZIP Library" und fügen die ZIP-Datei dort dem Projekt hinzu.
 *
 * Unter folgender URL finden Sie ein gutes Tutorial zur Nutzung der Bibliothek und I²S
 * mit dem ESP32 allgemein: https://dronebotworkshop.com/esp32-i2s/
 *
 * Lizenz
 * ======
 *
 * Die verwendete Bibliothek ESP32-audioI2S ist unter "GPL 3.0" lizenziert. Dies erfordert, dass
 * alle sie nutztenden Programme unter dieselbe oder eine neuere GPL-Lizenz fallen. Abweichend
 * zu den restlichen Beispielen ist dieser Quellcode daher unter der "GPL 3.0" oder nach Ihrer
 * Wahl einer neueren Version der GPL lizenziert.
 *
 * Weitere Audiostreams
 * ====================
 *
 * Grundsätzlich sollten Sie mit diesem Programm alle gängigen Audiostreams hören können,
 * deren URL Sie kennen. Im Quellcode sind ein paar URLs des SWR Baden-Württemberg hinterlegt.
 * Eine Übersicht über alle Livestreams des SWR finden Sie hier:
 *
 * https://www.swr.de/unternehmen/empfang/webradio-hoeren-alle-streams-100.html
 */

#include "wifi-wizard.h"
#include "serial-input.h"
#include <Audio.h>
#include <string.h>


// Vordefinierte Audio-Streams
typedef struct {
  char *name;
  char *url;
} audio_stream_t;

const audio_stream_t audio_streams[] = {
  {.name = "SWR1 BW",        .url = "https://liveradio.swr.de/sw282p3/swr1bw/"},
  {.name = "SWR3",           .url = "https://liveradio.swr.de/sw282p3/swr3/"},
  {.name = "DASDING",        .url = "https://liveradio.swr.de/sw282p3/dasding/"},
  {.name = "SWR1 Nr.1 Hits", .url = "https://liveradio.swr.de/sw331ch/raka16/"},
  {.name = "SWR4 Big Band", . url = "https://liveradio.swr.de/sw331ch/raka19/"},

  // Letzter Eintrag zum Erkennen des Listendes
  {.name = "", .url = ""}
};


// Audio-Objekt für die Tonausgabe
// #define USE_INTERNAL_DAC

#ifdef USE_INTERNAL_DAC
  Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);
#else
  constexpr uint8_t I2S_SCLK  = 18;
  constexpr uint8_t I2S_LRCLK = 19;
  constexpr uint8_t I2S_SDIN  = 5;
  constexpr  int8_t I2S_MCLK  = 3; // I2S_GPIO_UNUSED;

  Audio audio;
#endif

void setup() {
  // Serielle Verbindung starten
  delay(2000);
  Serial.begin(115200);

  // WLAN konifigurieren
  wifi_wizard();

  // Audio-Objekt initialisieren
  #ifdef USE_INTERNAL_DAC
    Serial.println("Nutze interne D/A-Wandler\n");
  #else
    Serial.println("Nutze externen I²S D/A-Wandler\n");
    audio.setPinout(I2S_SCLK, I2S_LRCLK, I2S_SDIN, I2S_MCLK);
  #endif

  audio.setVolume(21);  // Maximum: 21

  // Audiostream auswählen
  Serial.println("Verfügbare Audiostreams");
  Serial.println("=======================\n");

  int index = 0;
  const audio_stream_t *audio_stream = &audio_streams[0];

  while (strlen(audio_stream->name) > 0) {
    Serial.printf("[%i] %s\n", index, audio_stream->name);

    index += 1;
    audio_stream = &audio_streams[index];
  }

  String index_s = serial_input("\nBitte die Nummer des Audiostreams eingeben: ");
  Serial.println(index_s.c_str());

  index = atoi(index_s.c_str());
  audio_stream = &audio_streams[index];

  // Wiedergabe starten
  audio.connecttohost(audio_stream->url);
}

void loop() {
  audio.loop();
}
