"""
Hallo, Welt für Raspberry Pi
============================

Konfiguriert alle GPIO-Pins als digitale Ausgänge und schaltet diese
jede Sekunde ein und wieder aus. Dadurch blinkt die eingebaute LED des
Raspberry Pi sowie jedes angeschlossene Bauteil. Zusätzlich wird 
Meldung auf der Konsole ausgegeben.

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

    for pin in range(0, 26):
        gpio = DigitalOutputDevice(pin)
        gpios.append(gpio)

    for gpio in gpios:
        gpio.blink(on_time=0.5, off_time=0.5)

    print("Drücken Sie Strg+C zum Beenden!")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass
