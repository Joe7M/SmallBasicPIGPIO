![Logo SmallBASICPiGPIO](./images/logo_smallbasicpigpio.png)

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

### SSD1306 OLED Graphics Display added
February 20, 2022 - For details see: [SSD1306 OLED Display](./ssd1306.html)

***

## Setup and Pin numbering

- [Setup of SmallBasicPIGPIO](./setup.html)
- [Naming and Numbering of the GPIO-Pins](./gpio_numbering.html)
- [Setup I2C](./setupi2c.html)

## Commands by Topic

- [Basic GPIO commands](./gpio.html)
	- [GPIO_Pwm](./gpio.html#gpio_pwm) - [GPIO_Read](./gpio.html#gpio_read) - [GPIO_SetInput](./gpio.html#gpio_setinput) - [GPIO_SetOutput](./gpio.html#gpio_setoutput) - [GPIO_Trigger](./gpio.html#gpio_trigger) - [GPIO_Write](./gpio.html#gpio_write)
- [DS18B20 Temperature Sensor](./ds18b20.html)
	- [DS18B20_GetTemp](./ds18b20.html#ds18b20_GetTemp)
- [HD44780 compatible Text LCD Display](./hd44780.html)
	- [LCD1_Cls](./hd44780.html#lcd1_cls) - [LCD1_Init](./hd44780.html#lcd1_init) - [LCD1_Locate](./hd44780.html#lcd1_locate) - [LCD1_Off](./hd44780.html#lcd1_off) - [LCD1_On](./hd44780.html#lcd1_on) - [LCD1_Print](./hd44780.html#lcd1_print)
- [SSD1306 compatible OLED Display](./ssd1306.html)
    - [OLED1_Open](./ssd1306.html#oled1_open) - [OLED1_Close](./ssd1306.html#oled1_close) - [OLED1_Display](./ssd1306.html#oled1_display) - [OLED1_Cls](./ssd1306.html#oled1_cls) - [OLED1_Pset](./ssd1306.html#oled1_pset) - [OLED1_Line](./ssd1306.html#oled1_line) - [OLED1_Rect](./ssd1306.html#oled1_rect) - [OLED1_RoundRect](./ssd1306.html#oled1_roundrect) - [OLED1_Cirlce](./ssd1306.html#oled1_circle) - [OLED1_Triangle](./ssd1306.html#oled1_triangle) - [OLED1_Print](./ssd1306.html#oled1_print) - [OLED1_SetTextSize](./ssd1306.html#oled1_settextsize) - [OLED1_At](./ssd1306.html#oled1_at) - [OLED1_SetBrightness](./ssd1306.html#oled1_setbrightness) - [OLED1_InvertDisplay](./ssd1306.html#oled1_invertdisplay) - [OLED1_CopyToDisplay](./ssd1306.html#oled1_copytodisplay) - [OLED1_CopyFromDisplay](./ssd1306.html#oled1_copyfromdisplay)
- [SCD30 CO2, Temperature and Humidity Sensor](./scd30.html)
	- [SCD30_Open](./scd30.html#scd30_open) - [SCD30_Close](./scd30.html#scd30_close) - [SCD30_Start](./scd30.html#scd30_start) - [SCD30_Stop](./scd30.html#scd30_stop) - [SCD30_DataAvailable](./scd30.html#scd30_dataavailable) - [SCD30_ReadData](./scd30.html#scd30_readdata) - [SCD30_SetInterval](./scd30.html#scd30_setinterval) - [SCD30_SetTemperatureOffset](./scd30.html#scd30_settemperatureoffset) - [SCD30_SetCO2Recalibration](./scd30.html#scd30_setco2recalibration) - [SCD30_SetAltitudeRecalibration](./scd30.html#scd30_setaltituderecalibration) - 		[SCD30_AutomaticSelfCalibration](./scd30.html#scd30_automaticselfcalibration)

## Examples

- [LED](./example_led.html)
- [Push Button](./example_pushbutton.html)
- [DS18B20 Temperature Sensor](./example_ds18b20.html)
- [HD44780 Text LCD Display](./example_hd44780.html)
- [SSD1306 OLED Display](./example_ssd1306.html)
- [SCD30 CO2, Temperature and Humidity Sensor](./example_scd30.html)

## Links

- [SmallBasicPIGPIO at Github](https://github.com/Joe7M/SmallBasicPIGPIO)
- [Feedback for SmallBasicPIGPIO](https://github.com/Joe7M/SmallBasicPIGPIO/discussions)
- [SmallBASIC website](https://smallbasic.github.io/)
- [SmallBASIC forum](https://www.syntaxbomb.com/smallbasic/)
