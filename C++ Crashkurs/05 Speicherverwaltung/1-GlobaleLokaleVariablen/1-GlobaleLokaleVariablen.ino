/**
 * Globale und lokale Variablen
 * ============================
 *
 * Dieses und die folgenden Beispiele zeigen die verschiedenen Speicherklassen von C/C++,
 * also Orte, wo eine Variable im Programm existieren und verwendet werden kann. Mit den
 * objektorientierten Klassen hat dies trotz dem Namen nichts zu tun. :-)
 *
 * Dieses Beispiel zeigt die klassischen globalen und lokalen Variablen.
 */

// Variablen außerhalb einer Klasse, Struktur oder Funktion sind globale Variablen.
// Sie sind im gesamten Programm gültig und können überall verwendet werden.
int global_value = 0;

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  while (global_value++ < 5) {
    Serial.print(".");
  }

  Serial.println();
}

void loop() {
  if (global_value >= 5) {
    global_value = 0;

    long n = factorial(10);
    Serial.printf("10! = %lu\n", n);
  }
}


// Lokale Funktionen existieren nur auf dem Stack der sie umgebenden Funktion.
// Genauer gesagt liegen Sie innerhalb des "Stack Frame", das beim Aufruf der
// Funktion angelegt wird. Rekursive Funktionen wie diese hier erzeugen daher
// automatisch mehrere, sich nicht überschneidende Kopien der Variablen.
//
// Vgl. https://en.wikipedia.org/wiki/Man_or_boy_test
long factorial(long n) {
  if (n > 2) return n * factorial(n - 1);
  else return 1;
}