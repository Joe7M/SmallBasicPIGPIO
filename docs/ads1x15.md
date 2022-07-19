[back to main page](./index.html)



## ADS1015 and ADS1115 - Analog Voltage Sensor

The ADS1015 and ADS1115 are analog voltage sensors using the I2C-protocol for communication.
For setting up the Raspberry Pi for I2C and connecting the sensors please check the [example page](./example_ads1x15.html).


### ADS1x15_Open

> ADS1x15_Open(Address)

Address: optional parameter, sets the I2C address (0x48, 0x49, 0x50 or 0x51), default: 0x48

Connects to the ADS1x15 sensor.

```
ADS1x15_Open() 'opens connection with address 0x48
ADS1x15_Open(0x50) 'opens connection with address 0x50

```


### ADS1x15_Close

>ADS1x15_Close()

Closes the connection to the ADS1x15 sensor


### ADS1x15_SetChannel

>ADS1x15_SetChannel(Channel)

Channel: Input channel
  
Sets the input channel for measurement. Allowed channels are:

| 0 | Differential Input A0 - A1 |
| 1 | Differential Input A0 - A3 |
| 2 | Differential Input A1 - A3 |
| 3 | Differential Input A2 - A3 |
| 4 | Input A0 |
| 5 | Input A1 |
| 6 | Input A2 |
| 7 | Input A3 |


```
ADS1x15_SetChannel(4) ' Set input to channel A0
```


### ADS1x15_SetVoltageRange

>ADS1x15_SetVoltageRange(Range)

Range: Voltage range

Sets the Voltage range of the input. The ADS1015 and ADS1115 support the following voltage ranges:

| Voltage Range | Parameter |
|---------------|-----------|
| 0 to 256mV    | 0.256     |
| 0 to 512mV    | 0.512     |
| 0 to 1.024V   | 1.024     |
| 0 to 2.048V   | 2.048     |
| 0 to 4.096V   | 4.096     |
| 0 to 6.144V   | 6.144     |

The first range greater than or equal to the specified value will be set.

```
ADS1x15_SetVoltageRange(0)       ' 0 to 0.256V
ADS1x15_SetVoltageRange(1.024)   ' 0 to 1.024V
ADS1x15_SetVoltageRange(5)       ' 0 to 6.144V
```

### ADS1x15_SetSamplingRate

>ADS1x15_SetSamplingRate(SamplingRate)

SamplingRate: Sampling rate in samples per second

Sets the sampling rate. The ADS1015 supports 128, 250, 490, 920, 1600, 2400, and 3300 samples per second.
The ADS1115 supports 8, 16, 32, 64, 128, 250, 475, and 860 samples per second. The first sampling rate
greater than or equal to the specified value will be set.

```
ADS1x15_SetSamplingRate(0)       ' minimum sampling rate
ADS1x15_SetSamplingRate(128)     ' 128 samples per second
ADS1x15_SetSamplingRate(3300)    ' ADS1015: 3300 samples/s; ADS1115: 860 samples/s
```

[back to main page](./index.html)
