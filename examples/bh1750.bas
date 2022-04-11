' Example on how to measure the ambient light with a BH1750 sensor using SmallBASIC PiGPIO plugin
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2022
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin bh1750.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r bh1750.bas
'

import SmallBasicPIGPIO as gpio


gpio.BH1750_Open(0x23)

Light_HighResolution = gpio.BH1750_GetLight()
print "High resolution: "; Light_HighResolution; " lx"

gpio.BH1750_SetResolution(0)  '0: Low resolution, 1: High Resolution

Light_LowResolution = gpio.BH1750_GetLight()
print "Low resolution: "; Light_LowResolution; " lx"

gpio.BH1750_Close()



