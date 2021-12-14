'
' Example on how to set PWM (Pulse-Width Modulation) of a GPIO pin using the SmallBasicPIGPIO plugin.
' An LED can be connected to this pin. When PWM is active, the pin changes fast between low and high.
' The LED will switch on and off faster then the eye can resolve.
' The duty cycle determines the brightness of the LED. For example, if a duty cycle of 50% is set,
' the brightness of the LED will be half of the maximum brightness
'
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicGPIO/src/ pwm.bas
' SDL version: 		sudo sbasicg -m /home/pi/SmallBasicGPIO/src/ -r pwm.bas
'


import SmallBasicPIGPIO as gpio

const PIN_GPIO4 = 4

' Set pin as an output
gpio.GPIOSetOutput(PIN_GPIO4)

' Duty cycle is a value between 0 and 255.
' 0 -> LED is off
' 128 -> LED half brightness
' 255 -> LED max. brightness

for DutyCycle = 0 to 255 step 20

	print(DutyCycle)
	
	gpio.GPIOPwm(PIN_GPIO4, DutyCycle)
	
	delay(200)

next



