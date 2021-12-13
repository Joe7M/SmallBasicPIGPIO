If you want to let a LED blink or measure the temperature in your room, you can use [SmallBASIC](https://smallbasic.github.io) on a Raspberry PI now. As an example just connect a LED and a resistor to a GPIO-Pin of your Raspberry Pi, write a short SmallBASIC programm and enjoy the blinking LED.


```freebasic
import SmallBasicPIGPIO as gpio

const PIN_GPIO4 = 4

gpio.GPIOSetOutput(PIN_GPIO4)

for ii = 1 to 5
	gpio.GPIOWrite(PIN_GPIO4, 1)
	delay(500)
	gpio.GPIOWrite(PIN_GPIO4, 0)
	delay(500)
next
```
![Blinking LED](./images/blink.gif)

***

For more information on how to use SmallBasicPIGPIO please check the following links.

- [Setup of SmallBasicPIGPIO](./setup.html)
- [Naming and Numbering of the GPIO-Pins](./pinnumbering.html)

Commands by Topic

- [Basic GPIO commands](./gpio.html)
- [DS18B20 Temperature Sensor](./ds18b20.html)
- [HD44780 compatible Text LCD Display](./hd44780.html)

Examples

- [LED](./example_led.html)
- [Push Button](.example_pushbutton.html)
- [DS18B20 Temperature Sensor](./example_ds18b20.html)
- [HD44780 Text LCD Display](./example_hd44780.html)


- 
