/**
 * Atmega 328p als Display-Controller
 * ==================================
 *
 * GPIO-Pins sind bei fast allen Mikrocontrollern Mangelware. Das Problem wird allerdings
 * umso größer, je mehr Pins ein einzelnes Bauteil belegt. Ein typisches 16x2 Zeichen
 * Dot-Matrix-Display mit dem klassischen HD44780-Chip ist ein solches Beispiel, da es
 * sage und schreibe 10 Pins belegt. Einen sparen wir bereits, wenn wir nur Daten an das
 * Display senden aber nicht von dort lesen. Sonst wären es 11 Pins. Unabhängig davon
 * könnte man noch 4 Pins sparen, wenn man im 4-Bit-Modus mit dem Display kommuniziert.
 * Dieser ist allerdings sehr anfällig für Störungen und Timingprobleme, wodurch dann
 * nurnoch Datensalat angezeigt wird. Der 8-Bit-Modus ist da deutlich robuster.
 *
 * Dieses Programm ist deshalb dafür gedacht, ein kleines Controller-Board für ein Display,
 * einen Rotary Encoder (deutsch: Drehimpulsgeber) und einen Button (deutsch: Taster) zu
 * bauen. Das Programm wird hierfür auf einen Arduino Uno aufgespielt, dessen Atmega 328p
 * Mikrocontroller dann entfernt und auf ein Breadboard mit dem Display und den anderen
 * Komponenten gesetzt wird. In den Arduino muss dann ein neuer Chip eingesetzt werden.
 *
 * Das Programm kommuniziert mit einem Host-System über die serielle UART-Schnitstelle,
 * wobei die Nachrichten extrem simpel aufgebaut sind. Auf dem Host-System werden dadurch
 * nur zwei Pins zum Senden und Empfangen von Daten belegt, sofern man einen UART frei hat
 * oder einen Software-UART dafür nutzen kann. Die Details hierzu stehen weiter unten.
 *
 * Hardwareaufbau:
 * ---------------
 *
 * Siehe beigelegte Bilder. :-) Nachdem das Programm auf einen Arduino Uno aufgespielt wurde,
 * wird dessen Atmega 328p auf ein Breadboard gesetzt und mit Strom versorgt:
 *
 *  - Pin 1:  RESET -> 10k Ohm -> +5V
 *  - Pin 7:  VCC --------------> +5V
 *  - Pin 8:  GND --------------> Ground
 *  - Pin 20: AVCC -------------> +5V
 *  - Pin 21: Aref -------------> +5V
 *  - Pin 22: GND --------------> Ground
 *
 * Zusätzlich muss ein 16 MHz Quarzkristal eingesteckt werden:
 *
 *  - Pin 9:  XTAL1 -----> Kristal <-+
 *  - Pin 10: XTAL2 -----------------+
 *
 * Ein Rotary Encoder mit Push Button dient der Dateneingabe:
 *
 *  - Pin 23: A0 ------------> Button ------> Ground
 *
 *  - Pin 4:  Digital 2-----> Encoder A
 *                            Encoder COM -> Ground
 *  - Pin 5:  Digital 3 ----> Encoder B
 *
 * Zum Schluss folgt das Display:
 *
 *  - Pin 6:  Digital 4 --> RS
 *  - Pin 7:  Digital 5 --> E
 *  - Pin 8:  Digital 6 --> D0
 *  - Pin 13: Digital 7 --> D1
 *  - Pin 14: Digital 8 --> D2
 *  - Pin 15: Digital 9 --> D3
 *  - Pin 16: Digital 10 -> D4
 *  - Pin 17: Digital 11 -> D5
 *  - Pin 18: Digital 12 -> D6
 *  - Pin 19: Digital 13 -> D7
 *                          VSS -> Griound
 *                          VDD -> +5V
 *                          V0 --> Potentiometer -> +5V/Ground (einstellbarer Spannungsteiler!)
 *                          RW --> Ground
 *                          A ---> +5V
 *                          K ---> Ground
 *
 * Serielle Kommunikation
 * ----------------------
 *
 * Das fremde Mikrocontroller-Board muss wie folgt mit dem Atmega verbunden werden:
 *
 *  - Pin 2: RX (Daten empfangen) -> Mikrocontroller TX
 *  - Pin 3: TX (Daten senden) ----> Mikrocontroller RX
 *  - Pin 7: GND ------------------> Mikrocontroller GND
 *
 * Die Kommunikation basiert auf einem bidirektionalen Austausch von 1-Byte Nachrichten,
 * wobei manche Nachrichte noch Parameter beinhalten. Das Übertragungsformat ist 8N1
 * (Default bei Arduino) bei 9600 Baud Übertragungsrate.
 *
 * ### Mikrocontroller an Atmega
 *
 * | **Nachricht** | **Parameter** | **Bedeutung**                                      |
 * |---------------|---------------|----------------------------------------------------|
 * | 'I'           | Byte, Byte    | Init: Display initialisieren                       |
 * | 'C'           | Keine         | Clear Screen: Display leeren                       |
 * | 'P'           | String        | Print: Textzeile ausgeben                          |
 * | 'L'           | Byte, Byte    | Locate: Schreibposition/Cursor platzieren          |
 * | 'S'           | Boolean       | Show Cursor: Cursor für Texteingabe (nicht) zeigen |
 * | 'B'           | Boolean       | Blink Cursor: Cursor (nicht) blinken lassen        | 
 *
 * Die Parameter haben folgende Typen:
 *
 *  - String: Beliebige Zeichenkette gefolgt von '\n' (Zeilenendezeichen)
 *  - Byte: Vorzeichenlose Ganzzahl von 0 bis 255 als einzelnes Byte übertragen
 *  - Boolean: Beliebiges Byte, wobei 0 als False und alles andere als True gilt
 *
 * Ihre Werte sind:
 *
 *  - Init: Anzahl der Spalten und Zeilen des Displays
 *  - Print: Auszugebender Text
 *  - Locate: Spalte und Zeile der neuen Cursorposition (um hier z.B. Text anzuzeigen)
 *  - Cursor/Blink: True oder False
 *
 * ### Atmega an Mikrocontroller
 *;
 * | **Nachricht** | **Bedeutung**                                             |
 * |---------------|-----------------------------------------------------------|
 * | 'l'           | Left: Drehregler wurde einen Schritt nach links gedreht   |
 * | 'r'           | Right: Drehregler wurde einen Schritt nach rechts gedreht |
 * | 'b'           | Button: Der Knopf (im Drehregler) wurde gedrückt          |
 *
 * Hinweis zu Sonderzeichen
 * ========================
 *
 * Der HD44780 besitzt einen eigenen 8-Bit Zeichensatz, der die meisten ASCII-Zeichen, viele
 * japanische Kanji-Schriftzeichen und ein paar Sonderzeichen umfasst. Zusätzlich können bis
 * zu acht eigene Zeichen definiert werden. Das Programm geht davon aus, dass UTF-8 heute die
 * Standardcodierung auch für Arduino-Quellcodes ist und Sonderzeichen daher der Einfachheit
 * halber auch UTF-8 codiert über die serielle Schnittstelle übertragen werden. Dabei werden
 * folgende Zeichen auf den HD44780-Zeichensatz gemappt bzw. durch selbst-definierte Zeichen
 * abgebildet:
 *
 *   ä ö ü Ä Ö Ü ß \ ~ ← →
 *
 * Relevante Dokumentation
 * -----------------------
 *
 * https://docs.arduino.cc/language-reference/en/functions/communication/Serial/
 * https://docs.arduino.cc/libraries/liquidcrystal/
 */
 
#include <LiquidCrystal.h>

LiquidCrystal lcd(
  /* RS */ 4,
  /* RE */ 5,
  /* D0 */ 6,
  /* D1 */ 7,
  /* D2 */ 8,
  /* D3 */ 9,
  /* D4 */ 10,
  /* D5 */ 11,
  /* D6 */ 12,
  /* D7 */ 13
);

constexpr int encoder_a_pin = 2;
constexpr int encoder_b_pin = 3;
constexpr int button_pin    = A0;

bool sendMsg  = false;
char sendChar = 0;

/**
 * Initialisierung der Hardware nach dem Einschalten
 */
void setup() {
  pinMode(encoder_a_pin, INPUT_PULLUP);
  pinMode(encoder_b_pin, INPUT_PULLUP);
  pinMode(button_pin,    INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoder_a_pin), rotaryEncoderISR, RISING);

  while (!Serial);
  Serial.begin(9600);
}

/**
 * Unendlich oft ausgeführte Hauptprogrammlogik
 */
void loop() {
  // Prüfen, ob der Button gedrückt wurde
  if (digitalRead(button_pin) == LOW) {
    delay(100);

    if (digitalRead(button_pin) == LOW) {
      Serial.write('b');
    }
  }

  // Nachricht für Rotary Encoder senden, falls eine bereitsteht
  if (sendMsg) {
    Serial.write(sendChar);
    sendMsg = false;
  }

  // Nachricht empfangen und ausführen
  if (!Serial.available()) return;
  byte msg = Serial.read();

  switch (msg) {
    case 'I': {
      // Init
      char cols = receiveChar();
      char rows = receiveChar();
      lcd.begin(cols, rows);
      createSpecialChars();
      break;
    }
    case 'C': {
      // Clear Screen
      lcd.clear();
      break;
    }
    case 'P': {
      // Print
      lcd.print(mapSpecialChars(receiveString()).c_str());
      break;
    }
    case 'L': {
      // Locate
      char col = receiveChar();
      char row = receiveChar();
      lcd.setCursor(col, row);
      break;
    }
    case 'S': {
      // Show Cursor
      receiveChar() ? lcd.cursor() : lcd.noCursor();
      break;
    }
    case 'B': {
      // Blink Cursor
      receiveChar() ? lcd.blink() : lcd.noBlink();
      break;
    }
  }
}

/**
 * Interrupt Handler für den Rotary Encoder. Erkennt die Drehrichtung und legt dementsprechend
 * eine Nachricht in den globalen Variablen ab, damit diese in loop() gesendet werden kann.
 * Denn ein Interrupt Handler muss möglichst schnell zu Ende laufen und kann keine Dinge machen,
 * die selbst darauf angewiesen sind, dass Interrupts erkannt werden.
 */
void rotaryEncoderISR() {
  sendMsg = true;

  if (digitalRead(encoder_b_pin) == HIGH) {
    sendChar = 'r';
  } else {
    sendChar = 'l';
  }
}

/**
 * Hilfsfunktion zum Empfangen eines Byte-Parameters. Blockiert so lange, bis ein
 * Byte empfangen wurde und gibt es dann zurück.
 */
char receiveChar() {
  while (!Serial.available());
  return Serial.read();
}

/**
 * Hilfsfunktion zum Empfange
n eines Textstrings. Blockiert so lange, bis ein mit
 * '\n' terminierter String empfangen wurde und gibt ihn dann zurück.
 */
String receiveString() {
  while (true) {
    String result = Serial.readStringUntil('\n');
    if (result.length() > 0) return result.substring(0, result.length() - 1);
  }
}

/**
 * Sonderzeichen auf die in createSpecialChars() definierten Zeichen mappen.
 * Die Annahme hier ist, dass der Quellcode des nutzenden Programms UTF-8
 * codiert ist und die Umlaute daher auch in diesem Format gesendet werden.
 * Vgl. https://www.cogsci.ed.ac.uk/~richard/utf-8.cgi?mode=char
 */
String mapSpecialChars(String str) {
  String result = "";

  for (size_t i = 0; i < str.length(); i++) {
    char c1, c2, c3;
    c1 = str[i];

    switch (c1) {
      case '\\': result += '\x01'; break;  // Backslash (selbst definiert)
      case '~':  result += '\x02'; break;  // ~ (selbst definiert)

      // Zwei-Byte Sequenzen
      case '\xC3':
        if (i + 1 >= str.length()) continue;
        c2 = str[++i];

        switch (c2) {
          case '\xA4': result += '\xE1'; break;  // ä (eingebaut)
          case '\xB6': result += '\xEF'; break;  // ö (eingebaut)
          case '\xBC': result += '\xF5'; break;  // ü (eingebaut)
          case '\x84': result += '\x03'; break;  // Ä (selbst definiert)
          case '\x96': result += '\x04'; break;  // Ö (selbst definiert)
          case '\x9C': result += '\x05'; break;  // Ü (selbst definiert)
          case '\x9F': result += '\x06'; break;  // ß (selbst definiert)
        }

        break;

      // Drei-Byte Sequenzen
      case '\xE2':
        if (i + 2 >= str.length()) continue;
        c2 = str[++i];
        c3 = str[++i];

        switch (c2) {
          case '\x86':
            switch (c3) {
              case '\x90': result += '\x7F'; break;  // ← (eingebaut)
              case '\x92': result += '\x7E'; break;  // → (eingebaut)
            }

            break;
        }

        break;

      // Reguläre ASCII-Zeichen
      default:
        result += c1;
    }
  }

  return result;
}

/**
 * Der HD44780 kennt fast den gesamten ASCII-Zeichensatz bis auf Backslash und Tilde.
 * Außerdem kennt er die deutschen Buchstaben ä, ö, ü, ß und einige Sonderzeichen.
 * Unter anderem fehlen aber die groß geschriebenen Umlaute. Hier nutzen wir die Möglichkeit,
 * bis zu acht eigene Zeichen zu definieren, um die wichtigsten Zeichen nachzurüsten.
 */
void createSpecialChars() {
  byte specialChars[8][8] = {
    // Erstes ungenutzt, weil \x00 in einem C-String nicht vorkommen darf
    {},

    // Backslash
    {
      B00000,
      B10000,
      B01000,
      B00100,
      B00010,
      B00001,
      B00000,
    },

    // Tilde ~
    {
      B00000,
      B00000,
      B00000,
      B01101,
      B10010,
      B00000,
      B00000,
    },

    // Ä
    {
      B01010,
      B00000,
      B01110,
      B10001,
      B11111,
      B10001,
      B10001,
    },

    // Ö
    {
      B01010,
      B00000,
      B01110,
      B10001,
      B10001,
      B10001,
      B01110,
    },

    // Ü
    {
      B01010,
      B00000,
      B10001,
      B10001,
      B10001,
      B10001,
      B01110,
    },

    // ß
    {
      B01110,
      B10001,
      B11110,
      B10001,
      B11110,
      B10000,
      B10000,
    },

    {},
  };

  for (size_t i = 0; i < 8; i++) {
    lcd.createChar(i, specialChars[i]);
  }
}