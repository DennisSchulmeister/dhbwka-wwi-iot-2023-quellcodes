"""
Hallo, Welt für Raspberry Pi
============================

Konfiguriert alle GPIO-Pins als digitale Ausgänge und schaltet diese jede
Sekunde ein und wieder aus. Dadurch blinken alle angeschlossenen Bauteile.
Zusätzlich wird eine Meldung auf der Konsole ausgegeben.

Hardwareaufbau:
---------------

Keine Besonderheiten. Zum Testen kann eine LED oder Buzzer an einen der
Ausgänge gehängt werden.

Relevante Dokumentation
-----------------------

https://gpiozero.readthedocs.io/en/stable/api_output.html#digitaloutputdevice
"""

import time
from gpiozero import DigitalOutputDevice

def main():
    gpios = []
    value = 1

    for pin in range(0, 26):
        gpio = DigitalOutputDevice(pin)
        gpios.append(gpio)

    print("Drücken Sie Strg+C zum Beenden!")

    try:
        while True:
            if (value):
                print("Schalte alle Pins AN!")
            else:
                print ("Schalte alle Pins aus.")

            for gpio in gpios:
                gpio.value = value
            
            value = 0 if value == 1 else 1
            time.sleep(0.5)
    except KeyboardInterrupt:
        pass
