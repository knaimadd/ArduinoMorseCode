Arduino Morse code project.

Transmitter
Program that sends the sentence written (using keyboard) on Arduino Serial monitor using Morse Code. There is calibration that allows to set the speed of short signal (dot). Calibration also can be used to calibrate the receiver.

Receiver
Program that picks up the signal from transmitter and translates the morse code into readable sentence. Can also receive signal from button connected to this module.
A button in the middle is used to change source from photoresistor to button and vice versa. Yellow diode is an indicator of current source (on - photoresistor, off - button). Holding this button for one second begins the calibration process.
This module uses liquid crystal display with 2 rows of 16 charcter spaces. The potentiometer is used to manipulate contrast of the display.
