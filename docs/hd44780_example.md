[back to main page](./index.html)

# HD44780 Text Display Example

## Wiring

For running this example, you need a HD44780 compatible text display. Quite common are displays with 1, 2 or 4 lines, with
16 or 20 characters in each line. To adjust the contrast of the display a potentiometer with 10kOhms will be used.
If you have a display with a background LED illumination, then a LED resistor might be neccessary. If you don't aim for maximum 
brightness of the LED, then take a 220 Ohms or even a 1kOhm resistor.

![Wiring HD44780](./images/HD44780_wiring.png)

Connect the pins of the display to the following pins of the Raspberry Pi

Pin 1 (VSS) -> GND  
Pin 2 (VDD) -> 5V  
Pin 4 (RS)  -> GPIO5  
Pin 5 (R/W) -> GND  
Pin 6 (E) -> GPIO6  
Pin 7 (DB0) -> GND  
Pin 8 (DB1) -> GND  
Pin 9 (DB2) -> GND  
Pin 10 (DB3) -> GND  
Pin 11 (DB4) -> GPIO13  
Pin 12 (DB5) -> GPIO19  
Pin 13 (DB6) -> GPIO26  
Pin 14 (DB7) -> GPIO12  

Pin 3 (V0) -> Connect to the potentiometer  
  
If you have a background LED:  

Pin 15 (LED+) -> Connect to the LED resistor  
Pin 16 (LED-) -> GND  


## Software

```
import SmallBasicPIGPIO as gpio

const PIN_GPIO4 = 4

'Read temperature from sensor on GPIO4
Temp = gpio.DS18B20Temp(PIN_GPIO4)

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
```


[back to main page](./index.html)
