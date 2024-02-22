/**
 * Heap-Speicher
 * =============
 *
 * Neben den bisher kennengelernten Möglichkeiten, bieten C und C++ noch den Heap-Speicher
 * (auf deutsch manchmal Halde genannt), von dem zur Laufzeit neuer Speicher angefordert
 * werden kann, um darin Daten abzulegen. Auf mobilen und Desktopsystemen ist dieses
 * Verfahren gängige Praxis, kommt jedoch nicht ohne seine eigenen Schattenseiten:
 *
 *  - Jeder angeforderte Speicherbereich muss explit wieder freigegeben werden, damit es
 *    nicht zu einem "Speicherleck" kommt. Vgl. "Speicherverbrauch" eines Programms im
 *    Task Manager. Das ist der Heap-Speicher.
 *
 *  - Bereits freigegebener Speicher darf nicht mehr verwendet werden, um bizarre Bugs
 *    und Programmabstürze zu vermeiden.
 *
 *  - Auf Systemen mit begrenztem Hauptspeicher (heute: eingebettete Systeme wie der ESP32!)
 *    kann zu kleinteiliges Anfordern/Freigeben von Heap-Speicher zur "Speicherfragementierung"
 *    führen, was bedeutet, dass für eine Anforderung keine groß genuge Lücke mehr frei ist.
 *
 *  - Aufgrund der Suche eines freien Speicherbereichs zur Laufzeit, wenn diese angefordert
 *    wird, sind das Anforderung und Freigeben von Heap-Speicher nicht Realtime-Safe.
 *
 * Auf eingebetteten Systemen wird der Heap-Speicher daher in der Regel vermieden oder sehr
 * spärlich eingesetzt. Da sich C und C++ in der Syntax hier auch noch grundlegend unterscheiden,
 * wollen wir es aber dennoch ganz minimal betrachten.
 */

bool error = false; // Dummy für Beispiel unten

/**
 * Oldschool C: malloc() und free()
 */
void c_version() {
  char *data_buffer1, *data_buffer2;

  data_buffer1 = (char*) malloc(1024);   // 1 kB
  memset(data_buffer1, 0, sizeof(data_buffer1));
  // Hier irgendwas mit Buffer 1 machen
  if (error) goto cleanup1;

  data_buffer2 = (char*) malloc(1024);   // 1 kB
  memset(data_buffer2, 0, sizeof(data_buffer2));
  // Hier irgendwas mit Buffer 2 machen
  if (error) goto cleanup2;

cleanup2:
  free(data_buffer2);
cleanup1:
  free(data_buffer1);

  // ACHTUNG: Der Buffer darf ab hier nicht mehr verwendet werden!
  // Nicht bevor mit malloc() ein neuer Speicherblock angefordert wurde.

  // WEITERE STOLPERFALLE: free() darf nur aufgerufen werden, wenn der Zeiger
  // tatsächlich auf einen gültigen Speicherbereich zeigt. Sonst stürzt das
  // Programm wegen "double free" ab. Beachten Sie deshalb die umgedrehte
  // Cleanup-Reihenfolge oben.
}

/**
 * C++: Schlüsselwörter new und delete
 */
void cpp_version1() {
  char *data_buffer1, *data_buffer2;

  data_buffer1 = new char[1024];    // 1 kB
  memset(data_buffer1, 0, sizeof(data_buffer1));
  // Hier irgendwas mit Buffer 1 machen
  if (error) goto cleanup1;

  data_buffer2 = new char[1024];    // 1 kB
  memset(data_buffer2, 0, sizeof(data_buffer2));
  // Hier irgendwas mit Buffer 2 machen
  if (error) goto cleanup2;

cleanup2:
  delete data_buffer2;
cleanup1:
  delete data_buffer1;
}


// Da das obige Verfahren für C++ nicht wirklich besser als die C-Version und daher genauso
// fehleranfällig ist, haben sich Idiome wie RIIA entwickelt, um die Nutzung des Heaps sicherer
// und einfacher zu gestalten.
#include <memory>

void cpp_version2() {
  std::unique_ptr<char> data_buffer(new char[1024]);

  // Nichts weiter nötig. Der Unique Pointer liegt im Stack Frame der Funktion und kümmert
  // sich um die automatische Freigabe des Arrays im Heap, sobald die Funktion verlassen wird.
  // Das geht, weil Stack-Variablen bei Verlassen der Funktion automatisch "abgeräumt" werden,
  // indem ihre Destruktor-Methode aufgerufen wird. -> Smart Pointer!
}

// Am häufigsten werden new, delete, Smart Pointer zusammen mit Klassen genutzt, um große
// Objekte auf dem Heap abzulegen. Daher kommt auch das new-Schlüsselwort zur Erzeugung
// neuer Objekte in den meisten anderen Sprachen, da die Objekte dort auch auf dem Heap
// liegen. In C++ hat man allerdings die Auswahl an kann die Objekte auch als lokale
// Objekte im Stack ablegen. In eingebetteten Systemen kommt die dynamische Speicherverwaltung
// daher kaum zum Einsatz.


void setup() {
}

void loop() {
}