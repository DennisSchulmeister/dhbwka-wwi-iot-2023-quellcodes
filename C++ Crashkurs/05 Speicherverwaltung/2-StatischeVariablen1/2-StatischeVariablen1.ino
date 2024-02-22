/**
 * Statische Variablen
 * ===================
 *
 * Statische Variablen kennen Sie eventuell aus Java, wo mit `static` Variablen deklariert
 * werden, die an einer Klasse statt einer Objektinstanz hängen. Dies wurde direkt aus C++
 * übernommen. `static` funktioniert hier allerdings auch innerhalb einer Funktion, um
 * Variablen zu kennzeichnen, die ihren Wert nach dem Ende der Funktion beibehalten.
 *
 * Es handelt sich somit um eine elegante Alternative zu globalen Variablen, wenn diese nur
 * innerhalb einer einzigen Funktion benötigt werden.
 */


int countdown(int start) {
  static int number = start;
  return number--;
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  for (int i = 0; i < 10; i++) {
    Serial.printf("%i\n", countdown(10));
    delay(1000);
  }

  Serial.println("Lift off!");
}


void loop() {
}
