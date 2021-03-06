'
' Example on how to read the state of a GPIO pin using the SmallBasicPIGPIO plugin. 
' A push button can be connected to this pin to test, if the button was pressed.
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin button.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r button.bas
'

import SmallBasicPIGPIO as gpio

const PIN_GPIO17 = 17

' Set GPIO pin 17 to INPUT. When a pin is set to input,
' the internal pullup resistor will be enabled automaticly
gpio.GPIO_SetInput(PIN_GPIO17)

repeat 
	key = inkey()
	
	' Read the state of the pin. It will return 0 or 1.
	status = gpio.GPIO_Read(PIN_GPIO17)
	locate 1,1: print(status)
	
	showpage()
	delay(50)	
	
until key <> ""

