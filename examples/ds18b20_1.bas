'
' Example on how to read the temperature of a DS18B20 temperature 1-wire sensor using SmallBasicPIGPIO plugin
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicGPIO/src/ ds18b20.bas
' SDL version: 		sudo sbasicg -m /home/pi/SmallBasicGPIO/src/ -r ds18b20.bas
'

import SmallBasicPIGPIO as gpio

const PIN_GPIO22 = 22

'Read temperature from sensor on GPIO22
Temp = gpio.DS18B20Temp(PIN_GPIO22)

'Check for errors
if(Temp = -1000) then
	print("Could not find DS18B20 sensor.")
elseif(Temp = -2000) then
	print("Data from sensor was not read correctly.")
elseif(Temp = -3000) then
	print("Could not read sensor data.")
else
	print(Temp)
endif



