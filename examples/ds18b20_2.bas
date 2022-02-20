'
' Example on how to read the temperature of a DS18B20 temperature 1-wire sensor using SmallBasicPIGPIO plugin
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2021
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin ds18b20.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r ds18b20.bas
'

import SmallBasicPIGPIO as gpio

const PIN_GPIO22 = 22


while 1
    repeat
        'Read temperature from sensor on GPIO22
        Temp = gpio.DS18B20_GetTemp(PIN_GPIO22)
    until(Temp > -1000)

    print Temp
    delay(2000)
wend


