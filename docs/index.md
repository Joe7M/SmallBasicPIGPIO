![Logo SmallBASICPiGPIO](./images/logo_smallbasicpigpio.png)


__SmallBASIC PiGPIOv2 available, PiGPIOv1 is now depreciated.__

Please see [SmallBSSDIC PiGPIO2](https://joe7m.github.io/smallbasic.pigpio2).

If you want to let a LED blink or measure the temperature in your room, you can use [SmallBASIC](https://smallbasic.github.io) on a Raspberry PI now. As an example just connect a LED and a resistor to a GPIO-Pin of your Raspberry Pi, write a short SmallBASIC programm and enjoy the blinking LED.

```basic
import SmallBasicPIGPIO as gpio

const PIN_GPIO4 = 4

gpio.GPIO_SetOutput(PIN_GPIO4)

for ii = 1 to 5
	gpio.GPIO_Write(PIN_GPIO4, 1)
	delay(500)
	gpio.GPIO_Write(PIN_GPIO4, 0)
	delay(500)
next
```
![Blinking LED](./images/blink.gif)

***
## News

### SmallBASIC PiGPIOv2 available
[PiGPIOv2](https://joe7m.github.io/smallbasic.pigpio2/.) can be used on all Raspberry Pis from Zero to Pi 5. Version 1 is depreciated now.

### SmallBasicPiGPIO does not support Raspberry Pi 5
Februrar 14 2024: SmallBasicPiGPIO uses the library PiGPIO. PiGPIO supports only Raspberry Pi 1 to 4 and Pi Zero 1 and 2. At the moment I don't have a/the plan to port SmallBasicPiGPIO to the Raspberry Pi 5.

### Update of SmallBasicPiGPIO to new SmallBASIC version 12.27
August 7, 2023

### ADS1015 and ADS1115 analog voltage sensor added
July 19, 2022 - For details see: [ADS1x15 Voltage Sensor](./ads1x15.html)

### Advanced button example
Mai 17, 2022 - For details see [Push Button](./example_pushbutton.html)

### BH1750 Ambient Light Sensor added
April 11, 2022 - For details see: [BH1750 Ambient Light Sensor](./bh1750.html)


***
## Setup and Pin numbering

- [Setup of SmallBasicPIGPIO](./setup.html)
- [Naming and Numbering of the GPIO-Pins](./gpio_numbering.html)
- [Setup I2C](./setupi2c.html)

***
## Commands by Topic

- [Basic GPIO commands](./gpio.html): Input and output to the GPIO pins.
- [DS18B20 Temperature Sensor](./ds18b20.html): Measure the environment temperature.
- [HD44780 compatible Text LCD Display](./hd44780.html): Print text on a cheap but highly readable text-only display.
- [SSD1306 compatible OLED Display](./ssd1306.html): Draw graphics on this small and very cheap black and white OLED display.
- [BH1750 Ambient Light Sensor](./bh1750.html): Measure the ambient light.
- [ADS1015 and ADS1115 Voltage Sensor](./ads1x15.html): Measure voltages.
- [SCD30 CO2, Temperature and Humidity Sensor](./scd30.html): Powerfull and extrem precise sensor for measuring CO2, temperature and humidity.

***
## Examples

- [LED](./example_led.html)
- [Push Button](./example_pushbutton.html)
- [DS18B20 Temperature Sensor](./example_ds18b20.html)
- [HD44780 Text LCD Display](./example_hd44780.html)
- [SSD1306 OLED Display](./example_ssd1306.html)
- [BH1750 Ambient Light Sensor](./example_bh1750.html)
- [ADS1015 and ADS1115 Voltage Sensor](./example_ads1x15.html)
- [SCD30 CO2, Temperature and Humidity Sensor](./example_scd30.html)

***
## Projects

- [SmallBASIC Pi Console](./sbconsole.html)
<video width='200' controls loop autoplay muted>
	<source src='./images/SBConsole.mp4' type='video/mp4'>
</video>

***
## Links

- [SmallBasicPIGPIO at Github](https://github.com/Joe7M/SmallBasicPIGPIO)
- [Feedback for SmallBasicPIGPIO](https://github.com/Joe7M/SmallBasicPIGPIO/discussions)
- [SmallBASIC website](https://smallbasic.github.io/)
- [SmallBASIC forum](https://www.syntaxbomb.com/smallbasic/)
