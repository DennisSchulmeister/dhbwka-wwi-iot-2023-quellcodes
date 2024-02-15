/**
 * Variablen und Konstanten
 * ========================
 *
 * Dieser Sketch zeigt verschiedene Arten, wie Konstanten und Variablen deklariert
 * werden können und erklärt die Vor- und Nachteile.
 */

// Präprozessor-Makro: Wird beim Kompilieren durch den Text "16" ersetzt.
// Dies sieht man sehr häufig auch in C++ Quellcodes. Tatsächlich stimmt
// diese Art der Definition aber aus den C-Steinzeit und sollte in C++
// eher durch constexpr ersetzt werden.
#define LED_PIN 16

// Variable mit konstantem Wert: Da es eine Variable ist, liegt sie im Hauptspeicher
// und kann theoretisch ihren Wert ändern. Das const stellt nur sicher, dass über
// den hier definierten Namen keine Änderung zugelassen wird. Dies ist aber eine
// Prüfung zur Kompilierzeit, die leicht umgangen werden kann.
const int BUTTON1_PIN = 17;

// Eine echte Konstante, deren Wert zur Kompiliertzeit ermittelt wird. Der Wert
// kann sogar berechnet werden, wenn hier z.B. eine Formel angegeben wird. Der
// Kompiler führt die Formel aus und übernimmt nur das Endergebnis in den
// komilierten Maschinencode. Der Wert liegt nicht als Variable im Hauptspeicher.
// In C++ Quellcode ist dies dem älteren #define vorzuziehen.
constexpr int BUTTON2_PIN = 18;

// Initialisierung der Variable mit ihrem Standardwert.
bool led_value = {};


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
}

void loop() {
  if (digitalRead(BUTTON1_PIN))      led_value = false;
  else if (digitalRead(BUTTON2_PIN)) led_value = true;

  digitalWrite(LED_PIN, led_value);
}
