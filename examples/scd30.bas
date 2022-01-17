'
' Example on how to use a SCD30-sensor (temperature, humidity and CO2)
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2022
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ scd30.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -r scd30.bas
'

import SmallBasicPIGPIO as gpio

gpio.SCD30Open()
gpio.SCD30SetInterval(2) 'measurement interval 2s

for ii = 1 to 10

    'check if new data is available
    while(gpio.SCD30DataAvailable() != 1)
        delay(100)
    wend

    'Read the data. A array with three elements will be returnd.
    'Element 0: CO2 in ppm; Element 1: Temperature in °C; Element 2: Humidity in %RH

    Measurement = gpio.SCD30ReadData()

    print "CO2: "; round(Measurement(0)); "ppm | T: "; round(Measurement(1),1); "°C | H: "; round(Measurement(2));"%RH"

next

gpio.SCD30Close()


