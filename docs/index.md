If you want to let a LED blink or measure the temperature in your room, you can use [SmallBASIC](https://smallbasic.github.io) on a Raspberry PI now. As an example just connect a LED and a resistor to a GPIO-Pin of your Raspberry Pi, write a short SmallBASIC programm and enjoy the blinking LED.


```freebasic
import SmallBasicPIGPIO as gpio

const PIN_GPIO4 = 4

gpio.GPIOSetPin(PIN_GPIO4, 1)

for ii = 1 to 5
	gpio.GPIOWrite(PIN_GPIO4, 1)
	delay(500)
	gpio.GPIOWrite(PIN_GPIO4, 0)
	delay(500)
next
```

![Blinking LED](/docs/images/blink.gif)
