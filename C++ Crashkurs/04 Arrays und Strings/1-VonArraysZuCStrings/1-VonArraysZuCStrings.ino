/**
 * Arrays, C-Strings, String-Objekte
 * =================================
 *
 * Ein Nachteil von C/C++ gegenüber neueren Programmiersprachen ist, dass die Arbeit
 * mit Zeichenketten (Strings) weniger komfortabel ist. Dies liegt ist dem Alter von
 * C geschuldet, das bereits 1972 erschien und Zeichenketten im Wortsinne als eine
 * Kette (Array) von Zeichen betrachtet.
 *
 * Eine Errungenschaft von C++ war daraufhin die Möglichkeit, eine String-Klasse und
 * damit komfortablere und sicherere Möglichkeiten zur Stringverarbeitung zu ermöglichen.
 * Leider gab es im Laufe der Zeit aber viele verschiedene, zueinander inkompatible
 * Varianten der gleichen Idee und auch die Arduino-Bibliothek ist keine Ausnahme.
 * Sie stellt ihre eigene String-Klasse zur Verfügung, die unten ebenfalls vorgestellt wird.
 */

void setup() {
  delay(2000);
  Serial.begin(115200):
  Serial.print("\n=================================================\n");

  /**************************************
   * Von einzelnen Zeichen zu C-Strings *
   **************************************/

  // Ein einzelnes Zeichen. Der Typ char entspricht einem vorzeichenlosen 8-Bit Integer (uint8)
  // und kann daher die Zahlen von 0 bis 255 speichern.
  char zeichen = 'A';

  // Ein Array mit Zeichen
  char nachricht[7] = {'H', 'a', 'l', 'l', 'o', '!', '\n'};

  for (int i = 0; i < 7; i++) {
    zeichen = nachricht[i];
    Serial.print(zeichen);
  }

  int anzahlZeichen = sizeof(nachricht) / sizeof(char);

  // Array = Zeiger! Die folgende Zeile gibt die Speicheradresse des Arrays aus.
  Serial.printf("nachricht = %p\n", nachricht);

  for (int i = 0; i < 7; i++) {
    //       (2)          (1)
    zeichen = *(nachricht + i);   // 1 = Zeigerarithmethik, 2 = Dereferenzierung des Zeigers
    Serial.print(zeichen);
  }

  // C-String: Char-Array mit abschließendem Null-Byte
  char* nachricht2   = "Hallo, C-String!";
  char  nachricht3[] = {'H', 'a', 'l', 'l', 'o', '\0'};

  Serial.println(nachricht2);
  Serial.println(nachricht3);
  Serial.println("Ende!");
}

void loop() {
}
