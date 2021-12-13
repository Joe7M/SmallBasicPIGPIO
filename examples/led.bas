'
' Example on how to set the state of a GPIO pin using the SmallBasicPIGPIO plugin.
' An LED can be connected to this pin, to switch it on and off.
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicGPIO/src/ led.bas
' SDL version: 		sudo sbasicg -m /home/pi/SmallBasicGPIO/src/ -r led.bas
'

import SmallBasicPIGPIO as gpio

' LED is connected to pin GPIO4
const PIN_GPIO4 = 4

' Set pin as an output
gpio.GPIOSetOutput(PIN_GPIO4)

for ii = 1 to 5
	print(ii)
	'Set GPIO pin to high -> 3.3V
	gpio.GPIOWrite(PIN_GPIO4, 1)
	delay(500)
	'Set GPIO pin to low -> ground
	gpio.GPIOWrite(PIN_GPIO4, 0)
	delay(500)
next



