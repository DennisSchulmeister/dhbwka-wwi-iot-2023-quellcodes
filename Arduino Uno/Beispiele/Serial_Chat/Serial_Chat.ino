/**
 * Serial Chat
 * ===========
 *
 * Dieses Beispiel soll Ihnen die Möglichkeit geben, die serielle Kommunikation in einem eingebetteten
 * Mehrprozessorsystem (hier durch mehrere Arduino Uno Boards angedeutet) praktisch kennen zu lernen.
 * Das Programm realisiert hierfür eine einfache Chat-Anwendung, bei der beliebig viele Arduinos über
 * serielle Verbdinungen zu einer Kette zusammengeschaltet werden können, wobei jeder Arduino eine:r
 * "Benutzer:in" der Chat-Anwendung entspricht. In der seriellen Konsole der Arduino IDE kann auf jedem
 * Arduino eine Nachricht eingegeben werden, die über die Kette an alle anderen Arduinos weitergeleitet
 * und dort auf der seriellen Konsole ausgegeben wird.
 *
 * Probieren Sie es aus: Verbinden Sie mehrere Arduinos, wie unten gezeigt, und beobachten Sie, was
 * passiert, wenn Sie eine Nachricht abschicken.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Für dieses Experiment benötigen Sie mindestens zwei Arduino Boards und ein paar Steckdrähte.
 * Spielen Sie auf alle Arduinos dasselbe Programm auf und verbinden Sie die Boards über die
 * beiden software-emulierten seriellen Verbindungen wie folgt:
 *
 *   +-------------------+         +-------------------+         +-------------------+
 *   |        Pin 2 (RX) |<--------| Pin 5 (TX)    GND |<------->| GND    Pin 2 (RX) |<--- ...
 *   |        Pin 3 (TX) |-------->| Pin 4 (RX)        |         |        Pin 3 (TX) |---- ...
 *   |                   |         |                   |         |                   |
 *   |     Arduino 1     |         |     Arduino 2     |         |     Arduino 3     |
 *   |                   |         |                   |         |                   |
 *   |               GND |<------->| GND    Pin 2 (RX) |<--------| Pin 5 (TX)        |
 *   |                   |         |        Pin 3 (TX) |-------->| Pin 4 (RX)        |
 *   +-------------------+         +-------------------+         +-------------------+
 *
 * Pins 2 und 3 sind die Empfangs- und Sendepins des ersten Sofware Serial Ports, Pins 4 und 5
 * die des zweiten. Die Pins müssen daher immer über kreuz verbunden werden, damit der eine
 * Arduino empfängt, was der andere sendet. Das Programm ist so geschrieben, das eine auf einem
 * der Ports empfangene Nachricht an alle anderen weitergeleitet wird:
 *
 *   - Benutzereingabe via USB: Wir zur Anzeige an den USB-Port sowie an Sofware-Serial 1 und 2 geschickt
 *   - Nachricht auf Software-Serial 1: Wird an den USB-Port sowie an Software-Serial 2 geschickt
 *   - Nachricht auf Software-Serial 2: Wird an den USB-Port sowie an Software-Serial 3 geschickt
 *
 * Es ist daher egal, welche Software Serial Ports mit welchen der anderen Arduinos verbunden werden,
 * so lange alle Arduinos, wie oben gezeigt, eine Kette bilden.
 *
 * Hinweis zu Software Serial
 * --------------------------
 *
 * Die Emulation serieller Datenleitungen in Software statt in Hardware beansprucht einen großen Teil
 * der CPU-Ressourcen des Arduino Uno. Die verwendete Bibliothek "SoftwareSerial" hat deshalb eine
 * paar Einschränkungen, die dieses Beispiel leider etwas "unnötig" komplex machen:
 *
 *  - Daten können nicht gleichzeitig gesendet und empfangen werden. Senden und Empfang müssen
 *    zeitlich nacheinander erfolgen. Dadurch können Daten verloren gehen, da auf den Pins anliegende
 *    Signale einfach ignoriert werden, während ein Software-Serial Port nicht gelesen wird.
 *
 *  - Es kann immer nur ein Software-Serial Port gleichzeitig aktiv sein. Man kann also nicht
 *    mit zwei Software-Serial Ports gleichzeitig Daten empfangen. Stattdessen muss man mit der
 *    Methode `listen()` den Port "scharf" schalten, der gerade Daten empfangen soll. Zeitgleich
 *    anliegende Daten an allen anderen Software-Serial Ports gehen verloren.
 *
 * Hardware UARTs besitzen diese Einschränkungen nicht, doch leider besitzt der Arduino UNO aber
 * nur einen Hardware UART, der zusätzlich fest mit dem USB-Anschluss (und somit der seriellen
 * Konsole in der Arduino IDE) verdrahtet ist. Um dennoch eine halbwegs stabile Kommunikation über
 * die zwei benötigten Software-Serial Ports hinzubekommen, puffert das Programm jede auf einem
 * Port empfangene Nachricht und sendet diese dann hintereinander über die verbleibenden Ports
 * weiter.
 *
 * Damit die Gegenseite auch gerade "hinhört", wenn eine Nachricht gesendet werden soll, wird diese
 * mit einen Handshake-Protokoll über den Wunsch, Daten zu übertragen, informiert. Der Handshake
 * funktioniert dabei wie folgt:
 *
 *   1. Serielle Datenleitungen werden im Leerlauf vom Sender grundsätzlich immer auf HIGH gesetzt.
 *      Dies ist eine Eigenart fast aller serieller Schnittstellen.
 *
 *   2. Soll eine Nachricht über einen der Software-Serial Ports verschickt werden, meldet der
 *      sendende Arduino diesen Wunsch durch LOW-Setzen der Sendeleitung an.
 *
 *   3. Der empfangende Arduino registriert den LOW-Pegel und bestätigt dies, indem er seine
 *      Sendeleitung (also die Empfangsleitung des senden wollenden Arduinos) ebenfalls auf
 *      LOW setzt.
 *
 *   4. Der sendende Arduino registriert die Bestätigung und signalisiert dies, indem er seine
 *      Sendeleitung wieder auf HIGH setzt.
 *
 *   5. Der empfangende Arduino registriert dies und bestätigt, indem er seine Sendeleitung
 *      ebenfalls auf HIGH setzt.
 *
 *   6. Geschehen all diese Bestätigungen innerhalb einer festen Zeitspanne, gilt die Verbindung
 *      als aufgebaut. Der empfangende Arduino aktiviert den entsprechenden Software-Serial Port
 *      und der sendende Arduino überträgt die Nachricht.
 *
 *   7. Anschließend beendet der empfangende Arduino den Empfang auf dem Software-Serial Port.
 *
 * Der Kern dieser Logik stekct in den beiden Funktionen `request_communication()` und
 * `communication_requested()` ganz unten. In einer idealen Welt, ohne die Einschränkungen der
 * SofwareSerial-Bibliothek bräuchte es diesen Mechanismus nicht. :-)
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/communication/Serial/
 * https://docs.arduino.cc/learn/built-in-libraries/software-serial/
 */

#include <SoftwareSerial.h>

// Software Serial Ports
constexpr int software_serial1_rx = 2;
constexpr int software_serial1_tx = 3;
SoftwareSerial software_serial1(software_serial1_rx, software_serial1_tx);

constexpr int software_serial2_rx = 4;
constexpr int software_serial2_tx = 5;
SoftwareSerial software_serial2(software_serial2_rx, software_serial2_tx);

constexpr int timeout_handshake_ms = 100;
constexpr int timeout_receive_ms   = 500;

/**
 * Zeilenpuffer für empfangene Nachrichten. Je serieller Schnittstelle wird ein Objekt
 * dieser Klasse benötigt, um Chatnachrichten empfangen und zwischenspeicher zu können.
 */
class Buffer {
  public:
    static const int buffer_size = 255;
    char buffer[buffer_size] = {};
    size_t index = 0;
  
    /**
     * Methode zum Empfangen von Chatnachrichten über eine der seriellen Schnittstellen.
     * Prüft, ob ein neues Byte anliegt und schreibt es in den Puffer. Gibt true zurück,
     * wenn ein Zeilenumbruch und damit das Ende einer Nachricht erkannt wurde.
     */
    bool receive_message(Stream* stream, bool waitTillComplete) {
      unsigned long time1 = millis();

      while (true) {
        while (stream->available() > 0) {
          char c = stream->read();

          if (this->index < this->buffer_size) {
            this->buffer[this->index] = c;
            this->index++;

            if (c == '\n') {
              this->buffer[this->index - 1] = '\0';
              return true;
            }
          } else {
            this->buffer[this->buffer_size - 1] = '\0';
            return true;
          }
        }

        if (!waitTillComplete) return false;

        unsigned long time2 = millis();
        if ((time2 - time1) > timeout_receive_ms) return false;
      }
    }

    /**
     * Zurücksetzen des Puffers, nachdem die darin enthaltene Nachricht verarbeitet wurde.
     */
    void reset() {
      this->buffer[0] = '\0';
      this->index = 0;
    }
};

Buffer buffer0 = {};
Buffer buffer1 = {};
Buffer buffer2 = {};

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  while (!Serial);

  pinMode(software_serial1_rx, INPUT_PULLUP);
  pinMode(software_serial1_tx, OUTPUT);
  pinMode(software_serial2_rx, INPUT_PULLUP);
  pinMode(software_serial2_tx, OUTPUT);

  Serial.begin(9600);
  software_serial1.begin(9600);
  software_serial2.begin(9600);

  Serial.println("Bereit! Bitte Chatnachrichten eingeben und mit ENTER abschicken.\n");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  if (buffer0.receive_message(&Serial, false)) {
    Serial.print("Ich: ");
    Serial.println(buffer0.buffer);

    if (request_communication(software_serial1_rx, software_serial1_tx)) {
      software_serial1.println(buffer0.buffer);
    }

    if (request_communication(software_serial2_rx, software_serial2_tx)) {
      software_serial2.println(buffer0.buffer);
    }

    buffer0.reset();
  }

  if (communication_requested(software_serial1_rx, software_serial1_tx)) {
    software_serial1.listen();

    if (buffer1.receive_message(&software_serial1, true)) {
      Serial.print("Soft Serial 1: ");
      Serial.println(buffer1.buffer);

      if (request_communication(software_serial2_rx, software_serial2_tx)) {
        software_serial2.println(buffer1.buffer);
      }

      buffer1.reset();
    }

    software_serial1.stopListening();
  }
  
  if (communication_requested(software_serial2_rx, software_serial2_tx)) {
    software_serial2.listen();

    if (buffer2.receive_message(&software_serial2, true)) {
      Serial.print("Soft Serial 2: ");
      Serial.println(buffer2.buffer);

      if (request_communication(software_serial1_rx, software_serial1_tx)) {
        software_serial1.println(buffer2.buffer);
      }

      buffer2.reset();
    }

    software_serial2.stopListening();
  }
}

/**
 * Workaround für die Einschränkungen der SoftwareSerial-Bibliothek, dass immer nur ein Software
 * Serial Port aktiv sein kann und auch nur ein Port Daten empfangen kann. Damit die Gegenseite
 * weiß, dass sie auf einem Pin lesen soll, führen wir hier einen Handshake durch:
 *
 *  1. Unseren Sendepin auf HIGH ziehen
 *  2. Warten, bis die Gegenseite unseren Empfangspin auf HIGH zieht
 *  3. Unseren Sendepin auf LOW zurücksetzen
 *  4. Warten, bis die Gegenseite unseren Empfangspin auf LOW zurücksetzt
 *
 * Die Gegenseite hat für jeden Schritt `timeout_handshake_ms` Millisekunden Zeit. Sonst brechen
 * wir ab und gehgen davon aus, dass die Gegenseite nicht empfangsbereit ist.
 */
bool request_communication(int rx, int tx) {
  digitalWrite(tx, LOW);
  unsigned long time1 = millis();

  while (digitalRead(rx) == HIGH) {
    unsigned long time2 = millis();

    if ((time2 - time1) > timeout_handshake_ms) {
      digitalWrite(tx, HIGH);
      return false;
    }
  }

  digitalWrite(tx, HIGH);
  time1 = millis();

  while (digitalRead(rx) == LOW) {
    unsigned long time2 = millis();

    if ((time2 - time1) > timeout_handshake_ms) {
      return false;
    }
  }

  delay(timeout_handshake_ms / 2);
  return true;
}

/**
 * Gegenstück zur Funktion request_communication(). Prüft, ob der Empfangspin auf HIGH gezogen wird,
 * um eine Datenübertragung zu initiieren und bestätigt dies, indem der Sendepin ebenfalls auf HIGH
 * gezeogen wird. Dann wird gewartet, bis der Empfangspin wieder auf LOW geht, um den Sendepin danach
 * ebenfalls auf LOW zu ziehen. Geschieht dies vor Ablauf des Timeouts, machen wir uns bereit, auf
 * den jeweiligen Pins zu kommunizieren.
 */
bool communication_requested(int rx, int tx) {
  if (digitalRead(rx) == LOW) {
    digitalWrite(tx, LOW);
    unsigned long time1 = millis();

    while (digitalRead(rx) == LOW) {
      unsigned long time2 = millis();

      if ((time2 - time1) > timeout_handshake_ms) {
        digitalWrite(tx, HIGH);
        return false;
      }
    }

    digitalWrite(tx, HIGH);
    return true;
  }

  return false;
}
