"""
Hardware-Buttons entprellen
===========================

gpiozero macht es leicht, mit der Klasse DigitalInputDevice ein digitales Eingangssignal
auszulesen. Mechanische Bauteile wie z.B. Hardware-Buttons können aber nicht einfach
den Zustand von an nach aus oder umgekehrt wechseln. Sie pendeln bei jedem Wechseln
eine Weile zwischen beiden Zuständen hin und her, bis der mechanische Kontakt sicher
hergestellt wurde.

Idealzustand, den wir gerne hätten:

  Signal
   |...+                   +...
   |   |                   |
   |   |                   |
   |   +...................+
   +--------------------------> Zeit

 Reale Situation:

  Signal
   |....+  +..+  +..+      +..
   |    |  |  |  |  |      |
   |    |  |  |  |  |      |
   |    +..+  +..+  +......+..
   +--------------------------> Zeit

Mechanische Bauteile müssen daher "entprellt" werden (engl. "debounce"), indem bei
einem Wechsel des Signalzustands abgewartet wird, bis sich das Signal beruhigt hat.
Hierfür muss bei jedem registrierten Wechsel geprüft werden, wie lange der letzte
Wechsel her ist. Ist der zeitliche Abstand zum letzten Signalwechsel groß genug,
darf auf den neue Zustand reagiert werden. gpiozero beinhaltet hierfür die Klasse
Button, die genau ein solches Software-Debouncing vornimmt.

Das Programm fragt einfach einen an GPIO 2 angeschlossenen Button ab, der auf Ground
durchschalten muss, wenn er gedrückt wird, und schaltet eine an GPIO 17 angeschlossene
LED entsprechend ein oder aus.

Hardwareaufbau:
---------------

 - [GPIO 2] --> [Button] --> [GND]
 - [GPIO 17] --> [LED] --> [Widerstand 300 Ohm] --> [GND]

Für den Button-Pin wird der eingebaute Pull-Up-Widerstand des Raspberry Pi aktiviert.
Der Button muss das Signal daher beim Drücken auf Ground runterziehen, wodurch sich
normalerweise die Logik im Programm invertieren würde: Wir lesen ein OFF, wenn der
Button gedrückt wird und ein ON, wenn er nicht gedrückt wird. gpiozerio berücksichtig
das aber bereits für uns und liefert immer ein ON, wenn der Butten gedrückt wird.

Die LED zeigt, ob der Button gerade gedrückt wird. Falls das X40 Sensor Kit verwendet
wird, wird kein Widerstand für die LED benötigt, da dieser bereits auf dem LED-Modul
enthalten ist. Andernfalls kann ein beliebiger Widerstand ab ca. 300 Ohm verwendet werden,
um die Stromstärke für die LED zu begrenzen.

Relevante Dokumentation
-----------------------

https://gpiozero.readthedocs.io/en/stable/api_input.html#button
https://gpiozero.readthedocs.io/en/stable/api_output.html#led
"""

import signal
from gpiozero import Button, LED

def main():
    button = Button(2, pull_up=True, bounce_time=0.1)
    led    = LED(17)

    def button_callback():
        led.value = button.value

    button.when_pressed  = button_callback
    button.when_released = button_callback

    print("Drücken Sie Strg+C zum Beenden")

    try:
        signal.pause()
    except KeyboardInterrupt:
        pass
