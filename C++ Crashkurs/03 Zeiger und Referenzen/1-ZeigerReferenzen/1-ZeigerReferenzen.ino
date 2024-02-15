/**
 * Zeiger und Referenzen
 * =====================
 *
 * Für Java-Umsteiger ungewohnt ist das Konzept von Zeigern und Referenzen in C/C++.
 * Im Grunde ist aber ganz leicht:
 *
 *   - Zeiger: Beinhaltet die Speicheradresse einer Variable (und "zeigt" somit auf diese)
 *   - Referenz: Ist ein anderer Name für eine bereits vorhandene Variable
 *
 * Referenzen werden hauptsächlich benötigt, wenn man Werte an eine Funktion übergeben
 * will, ohne die Werte hierfür zu kopieren. Die aufgerufene Funktion arbeitet dann
 * einfach mit genau derselben Variable wie die rufende Funktion, benutzt ggf. nur einen
 * anderen Namen dafür.
 *
 * Zeiger werden überwiegend dann benötigt, wenn eine Variable oder ein Objekt zwar innerhalb
 * einer Funktion erzeugt wird, nach dem Verlassen der Funktion aber weiter existieren muss.
 * In diesem Fall legt man die Variable oder das Objekt auf dem "Heap" (Deutsch: Haldenspeicher)
 * an und lässt sich einen Zeiger darauf geben.
 */


/**
 * Call by Value: Der Parameter "value" erhält eine Kopie des übergebenen Werts
 */
void call_by_value(int value) {
  value = 22;
}


/**
 * Call by Reference (Und-Zeichen): Der Parameter "value" verwendete die Variable des
 * Aufrufers und kann auch ihren Wert überschreiben.
 */
void call_by_reference(int& value) {
  value = 33;
}


/**
 * Hat keine offiziellen Name, Übergabe eines Pointers (Sternchen): "value" enthält die Speicheradresse(!)
 * des übergebenen Werts. Um den Wert zu verwenden, muss die Speicheradresse "dereferenziert" werden.
 * Da der Zeiger auf die Variable des Aufrufers zeigt, kann der Wert geändert werden.
 */
void call_by_pointer(int* value) {
  *value = 44;

  // Zeiger in Referenz umwandeln
  int& value_ref = *value;
  value_ref = 55;
}


void setup() {
  Serial.begin(115200);

  int value = 11;
  Serial.printf("Value bei Programmstart: %u\n", value);

  call_by_value(value);
  Serial.printf("Value nach call_by_value(value): \%u\n", value);

  call_by_reference(value);
  Serial.printf("Value nach call_by_reference(value): \%u\n", value);

  call_by_pointer(&value);
  Serial.printf("Value nach call_by_pointer(&value): \%u\n", value);
}

void loop() {
}