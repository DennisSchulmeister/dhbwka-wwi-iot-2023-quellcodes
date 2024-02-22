/**
 * Statische Variablen zum Debouncen eines Hardware-Buttons
 * ========================================================
 *
 * Dieses Beispiel zeigt, wie statische Variablen genutzt werden können, um einen einzelnen
 * Hardwarebutton zu debouncen. So simpel wie das Beispiel jedoch ist, desto mehr Probleme
 * hat es jedoch: Während dem Debouncing wird der aktuelle Task blockiert und es kann auf
 * diese Weise nur ein einziger Button abgefragt werden, da die statische Variable ja im
 * gesamten Programm nur einmal existiert.
 *
 * HINWEIS: Im Einparkhilfe-Beispiel für den Arduino Uno befindet sich eine erweiterte
 * Version der Funktion read_button(), die bis zu 20 Buttons unterscheiden kann.
 */

const int BUTTON_PIN = 23;    // Ganz oben rechts


bool read_button(int pin) {
  static bool debounced_value;
  auto current_value = digitalRead(pin);

  while (current_value != debounced_value) {
    delay(10);
    debounced_value = current_value;
    current_value = digitalRead(pin);
  }

  return current_value;
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.print("\n=================================================\n");

  Serial.println("Bitte Button an GPIO23 gegen GND schließen.");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  static int i = 0;

  if (!read_button(BUTTON_PIN)) {
    i = ++i % 20;
    if (i == 0) Serial.println();

    Serial.print(".");
  }
}
