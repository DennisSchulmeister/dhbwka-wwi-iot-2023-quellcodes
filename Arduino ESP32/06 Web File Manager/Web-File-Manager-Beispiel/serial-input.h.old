#pragma once

/**
 * Hilfsfunktion zum Einlesen einer Textzeile über die serielle Konsole
 */
String serial_input(const char* prompt, bool blocking = true) {
  String result = {};

  Serial.print(prompt);

  while (result.length() == 0) {
    result = Serial.readStringUntil('\n');
    result.trim();

    if (!blocking) break;
  }

  return result;
}