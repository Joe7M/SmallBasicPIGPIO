'
' Example on how to emit a trigger pulse on a GPIO pin using the SmallBasicPIGPIO plugin.
' An LED can be connected to this pin. The LED should be on for a very shrt time.
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin trigger.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r trigger.bas
'

import SmallBasicPIGPIO as gpio

' LED is connected to pin GPIO4
const PIN_GPIO4 = 4

' Set pin as an output
gpio.GPIO_SetOutput(PIN_GPIO4, 1)

for ii = 1 to 5
	print(ii)
	'Emit a trigger pulse with 50 microseconds pulse length and with high level
	gpio.GPIO_Trigger(PIN_GPIO4, 50, 1)
	delay(500)
next



