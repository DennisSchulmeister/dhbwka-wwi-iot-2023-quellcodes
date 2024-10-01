/**
 * Timer Interrupt
 * ===============
 *
 * Der ESP32 hat je nach Variante zwei oder vier eingebaute Hardware-Timer, die für
 * sehr präzise Zeitmessungen verwendet werden können. Wie für Hardware-Timer üblich,
 * handelt es sich dabei um Counter, die in einer festen Geschwindigkeit eine Zahl
 * hoch oder runter zählen und bei Erreichen eines bestimmten Wertes einen Interrupt
 * auslösen, der die CPU in ihrer Ausführung unterbricht.
 *
 * Hardware-Timer kommen daher immer dann zum Einsatz, wenn die eingebauten Funktionen
 * delay() und delayMicroseconds() der Arduino-Platform zu ungenau sind oder wenn Jitter
 * aufgrund variable lange dauernder Aktionen nach einem delay() vermieden werden sollen.
 *
 * Funktionsweise eines Hardware-Timers
 * ====================================
 *
 * Die Hardware-Timer des ESP32 werden von einem 80 MHz Clock-Signal angetrieben.
 * Sie können daher exakt 80 Million mal pro Sekunde ihren Wert hoch/runter zählen.
 * Beim Einrichten des Timers wird diese Geschwindigkeit durch einen "Divisor" (manchmal
 * auch "Pre-Scaler" genannt) geteilt, um sie zu verlangsamen. Siehe Zeile 43.
 *
 * Mit dieser Einstellung läuft der Timer unendlich lange und zählt einfach eine
 * Zahl hoch (Zeile 47). Sobald dabei ein bestimmter Wert erreicht wird (Zeile 59),
 * wird ein Interrupt ausgelöst und der Timer wieder zurückgesetzt.
 *
 * Soll ein Timer regelmäßig alle x Milisekunden auslösen, kann dem Timer das gewünschte
 * Intervall gar nicht direkt mitgegeben werden. Stattdessen muss man anhand der Clock-
 * Frequenz Divisor und Zielwert ausrechnen und diese der Hardware übergeben.
 *
 * ESP32/Arduino API
 * =================
 *
 * Das nachfolgende Beispiel zeigt, wie die timer-Funktionen des ESP32 Ardiuno Cores
 * verwednet werden können. Es handelt sich hierbei um die Stand April 2024 noch aktuelle
 * API Version 2. Die Dokumenationsseite des ESP32 Arduino Core beschreibt hingegen schon
 * die stark vereinfachten Schnittstellen ab Version 3, die bisher aber noch nicht
 * freigegeben wurde.
 *
 * Ab Version 3 entfällt das manuelle Ausrechnen von Divisor und Zählwert, so dass man
 * direkt das gewünschte Zeitintervall übergeben kann.
 */

#include <Arduino.h>

hw_timer_t *timer = 0;
bool led = false;

void setup() {
  pinMode(2, OUTPUT);

  timer = timerBegin(
    /* frequency */ 1000000        // Zählgeschwindigkeit in Hz (sollte ca. 1 MHz betragen)
  );

  timerAttachInterrupt(
    /* timer    */ timer,          // Timer-Struktur
    /* userFunc */ &onTimer        // Callback-Funktion
  );

  timerAlarm(
    /* timer       */  timer,      // Timer-Struktur
    /* alarm_value */  1000000,    // Bis zu welchem Wert der Timer zählen soll
    /* autoreload  */  true,       // Timer automatisch neustarten
    /* reload_count */ 0           // Wie oft der Timer neugestartet werden soll (0 = unbegrenzt)
  );

  timerStart(timer);
}


void loop() {
}


/**
 * VORSICHT: Dies ist ein Interrupt Handler. Hier keine langwierigen Dinge machen, da dies
 * die anderen Tasks der CPU blockiert. Bestimmte Dinge wie z.B. serial.println() bringen
 * den ESP32 sogar zum Absturz!
 */
void ARDUINO_ISR_ATTR onTimer() {
  led = !led;
  digitalWrite(2, led);
}
