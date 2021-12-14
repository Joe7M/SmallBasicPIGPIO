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
- [Naming and Numbering of the GPIO-Pins](./gpio_numbering.html)

Commands by Topic

- [Basic GPIO commands](./gpio.html)
	- [GPIOPwm](./gpio.html#gpiopwm) - [GPIORead](./gpio.html#gpioread) - [GPIOSetInput](./gpio.html#gpiosetinput) - [GPIOSetOutput](./gpio.html#gpiosetoutput) - [GPIOTrigger](./gpio.html#gpiotrigger) - [GPIOWrite](./gpio.html#gpiowrite)
- [DS18B20 Temperature Sensor](./ds18b20.html)
	- [DS18B20Temp](./ds18b20.html#ds18b20temp)
- [HD44780 compatible Text LCD Display](./hd44780.html)
	- [LCD1Cls](./hd44780.html#lcd1cls) - [LCD1Init](./hd44780.html#lcd1init) - [LCD1Locate](./hd44780.html#lcd1locate) - [LCD1Off](./hd44780.html#lcd1off) - [LCD1on](./hd44780.html#lcd1on) - [LCD1Print](./hd44780.html#lcd1print) 

Examples

- [LED](./example_led.html)
- [Push Button](.example_pushbutton.html)
- [DS18B20 Temperature Sensor](./example_ds18b20.html)
- [HD44780 Text LCD Display](./example_hd44780.html)

