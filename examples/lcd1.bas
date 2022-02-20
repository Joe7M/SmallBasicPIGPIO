'
' Example on how to use an HD44780 compatibler LCD with the SmallBasicPIGPIO plugin.
' A LCD display with an HD44780 compatible controller can be used in 4 bit mode
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin lcd1.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r lcd1.bas
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
gpio.LCD1_Init(PIN_RS, PIN_E, PIN_D4, PIN_D5, PIN_D6, PIN_D7)

'Clear LCD and move cursor to position (1,1)
gpio.LCD1_Cls()
gpio.LCD1_Print("Test1")

'Move cursor to position (5,2)
gpio.LCD1_Locate(5,2)
gpio.LCD1_Print("Test2")

'Turn LCD off and on
delay(1000)
gpio.LCD1_Off()
delay(1000)
gpio.LCD1_On()

