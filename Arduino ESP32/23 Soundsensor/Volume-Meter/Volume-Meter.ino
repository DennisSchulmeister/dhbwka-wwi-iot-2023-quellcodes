/**
 * Volume Meter
 * ============
 */

#include <Arduino.h>

hw_timer_t *timer = 0;
bool led = false;

void setup() {
  pinMode(2, OUTPUT);

  timer = timerBegin(
    /* num     */ 0,            // Hardwaretimer 0 bis 3
    /* divider */ 800,          // Zählgeschwindigkeit: 80 MHz / divider
    /* countUp */ true          // Zählrichtung
  );

  timerAttachInterrupt(
    /* timer */ timer,          // Timer-Struktur
    /* fn    */ &onTimer,       // Callback-Funktion
    /* edge  */ false           // Muss false sein
  );

  timerAlarmWrite(
    /* timer       */ timer,    // Timer-Struktur
    /* alarm_value */ 100000,   // Bis zu welchem Wert der Timer zählen soll
    /* autoreload  */ true      // Timer automatisch neustarten
  );

  timerAlarmEnable(timer);
}


void loop() {
}


void ARDUINO_ISR_ATTR onTimer() {
  led = !led;
  digitalWrite(2, led);
}