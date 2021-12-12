'
' Example on how to use an HD44780 compatibler LCD with the SmallBasicPIGPIO plugin.
' A LCD display with an HD44780 compatible controller can be used in 4 bit mode
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ lcd1.bas
' SDL version: 		sudo sbasicg -m /home/pi/SmallBasicGPIO/bin/ -r lcd1.bas
'

import SmallBasicPIGPIO as gpio

' The display is connected to the following pins
const PIN_RS = 5
const PIN_E = 6
const PIN_D4 = 13
const PIN_D5 = 19
const PIN_D6 = 26
const PIN_D7 = 12

'Init the display
'gpio.LCD1Init(PIN_RS, PIN_E, PIN_D4, PIN_D5, PIN_D6, PIN_D7)

for ii = 1 to 20
	gpio.LCD1Print("a")
	delay(300)
next




