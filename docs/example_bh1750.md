[back to main page](./index.html)

# BH1750 Ligh Ambient Sensor - Example

## Wiring

For running this example, you need a BH1750 sensor. SmallBASICPiGPIO is using the I2C-protocol for communication.
The Raspberry Pi support this protocol in hardware, but by default the protocol is disabled. Therefore you have to
setup I2C as described [here](./setupi2c.html). In the next step please wire the sensor as shown in the following image.

![Wiring bh1750](./images/bh1750_wiring.png)

The I2C bus is using pin 2 (SDA1) and 3 (SCL1). Please be carefull, the sensors are usually driven with 3.3V. The sensor
from Adafruit can be driven with 3.3V and 5V. If you don't connect the address pin, then the sensor will use address 0x23.

## Software

```freebasic
import SmallBasicPIGPIO as gpio


gpio.BH1750_Open() 'Address 0x23, high resolution

Light_HighResolution = gpio.BH1750_GetLight()

gpio.BH1750_SetResolution(0)  '0: Low resolution, 1: High Resolution

Light_LowResolution = gpio.BH1750_GetLight()

print "High resolution: "; Light_HighResolution; " lx"
print "Low resolution: "; Light_LowResolution; " lx"

gpio.BH1750_Close()
```

[back to main page](./index.html)
