/**
 * Serial Chat
 * ===========
 *
 * xxx
 *
 * Hardwareaufbau:
 * ---------------
 *
 * xxx
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/communication/Serial/
 * https://docs.arduino.cc/learn/built-in-libraries/software-serial/
 */

#include <SoftwareSerial.h>

// Software Serial Ports
constexpr int serial1_rx = 2;
constexpr int serial1_tx = 3;
SoftwareSerial serial1(serial1_rx, serial1_tx);

constexpr int serial2_rx = 4;
constexpr int serial2_tx = 5;
SoftwareSerial serial2(serial2_rx, serial2_tx);

constexpr int timeout_handshake_ms = 100;
constexpr int timeout_receive_ms   = 500;

/**
 * Zeilenpuffer für empfangene Nachrichten. Je serieller Schnittstelle wird ein Objekt
 * dieser Klasse benötigt, um Chatnachrichten empfangen zu können.
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

  pinMode(serial1_rx, INPUT_PULLUP);
  pinMode(serial1_tx, OUTPUT);
  pinMode(serial2_rx, INPUT_PULLUP);
  pinMode(serial2_tx, OUTPUT);

  Serial.begin(9600);
  serial1.begin(9600);
  serial2.begin(9600);

  Serial.println("Bereit! Bitte Chatnachrichten eingeben und mit ENTER abschicken.\n");
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  if (buffer0.receive_message(&Serial, false)) {
    Serial.print("Ich: ");
    Serial.println(buffer0.buffer);

    if (request_communication(serial1_rx, serial1_tx)) {
      serial1.println(buffer0.buffer);
    }

    if (request_communication(serial2_rx, serial2_tx)) {
      serial2.println(buffer0.buffer);
    }

    buffer0.reset();
  }

  if (communication_requested(serial1_rx, serial1_tx)) {
    serial1.listen();

    if (buffer1.receive_message(&serial1, true)) {
      Serial.print("Soft Serial 1: ");
      Serial.println(buffer1.buffer);

      if (request_communication(serial2_rx, serial2_tx)) {
        serial2.println(buffer1.buffer);
      }

      buffer1.reset();
    }

    serial1.stopListening();
  }
  
  if (communication_requested(serial2_rx, serial2_tx)) {
    serial2.listen();

    if (buffer2.receive_message(&serial2, true)) {
      Serial.print("Soft Serial 2: ");
      Serial.println(buffer2.buffer);

      if (request_communication(serial1_rx, serial1_tx)) {
        serial1.println(buffer2.buffer);
      }

      buffer2.reset();
    }

    serial2.stopListening();
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