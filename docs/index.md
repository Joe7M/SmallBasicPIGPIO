If you want to turn on a LED and let it blink, you can use SmallBASIC on a Raspberry PI now. Just connect a LED to a GPIO-Pin of a Raspberry Pi and write a short SmallBASIC programm:
```FreeBasic
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
