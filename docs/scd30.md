[back to main page](./index.html)

## SCD30 - CO2, Temperature and Humidity Sensor

The SCD30 is an CO2, Temperature and Humidity sensor using the I2C-protocol for communication.
For setting up the Raspberry Pi for I2C and connecting the sensor please check the [example page](./example_scd30.html).

### SCD30Open
```
SCD30Open()
```
Connects to the SCD30 sensor.

### SCD30Close
```
SCD30Close()
```
Closes the connection to the SCD30 sensor

### SCD30Start
```
SCD30Start()
SCD30Start(Pressure)
```
Pressure: optional parameter to set the ambient air pressure in mBar  
Pressure = 0 -> Pressure compensation will be disabled  
Pressure = 700..1200 -> ambient air pressure in mBar  
  
Starts continuous measurement with optional ambient air pressure compensation  

The CO2 measurement value can be compensated for ambient air pressure by feeding the pressure value in mBar to the sensor.
Setting the ambient pressure will overwrite previous and future settings of altitude compensation. Setting the argument
to zero will deactivate the ambient pressure compensation.

```
SCD30Start() 'Starts measurement with deactivated ambient pressure compensation
SCD30Start(0) 'Starts measurement with deactivated ambient pressure compensation
SCD30Start(1010) 'Starts measurement with activated ambient pressure compensation of 1010mBar
```

### SCD30Stop
```
SCD30Stop()
```
Stops the continuous measurement. This command can be used to save energy. To keep stable and reliable measurement conditions, let the sensor always run.

### SCD30DataIsAvailable
```
DataIsAvailable = SCD30DataIsAvailable()
```
DataIsAvailable = 0 or 1

Returns 1, if a measurement is completed and the data can be read, otherwise 0.

```
while(SCD30DataIsAvailable() != 1)
  delay(100)
wend
```


### SCD30ReadData
```
M = SCD30ReadData()
```
M(0) = CO2  
M(1) = Temperature  
M(2) = Humidity  

Reads the data of the last measurement and returns an array with three elements. First element is CO2 in ppm; 
second elements is temperature in °C; third element is humidity in %RH

```
Measurement = SCD30ReadData()
print "CO2: "; Measurement(0)
print "Temperature: "; Measurement(1)
print "Humidity: ";Measurement(2)
```
### SCD30SetInterval
```
SCD30SetInterval(Interval)
```
Interval = 2...1800

Sets the interval in seconds between the measurements. Interval must be at leat 2s, max. 1800s.
```
SCD30SetInterval(2)
```

### SCD30SetTemperatureOffset
**Expert command**
```
SCD30SetTemperatureOffset(Offset)
```
Offset >= 0

The on-board humidity and temperature sensor is influenced by thermal self-heating of 
SCD30 and other electrical components. Design-in alters the thermal properties of
SCD30 such that temperature and humidity offsets may occur when operating the sensor
in end-customer devices. Also changing the measurement intervall might result in different
heat contribution in the sensor. Compensation of those effects is achievable by writing the 
temperature offset found in continuous operation of the device into the sensor.
Temperature offset value is saved in non-volatile memory. The last set value will be
used for temperature offset compensation after repowering.

The new temperature offset can be calculated using following equation:
```
T_Offset,New = (T_SCD30 − T_Reference ) + T_Offset,Old
````
T_SCD30: actual temperature reading of the sensor  
T_Reference: ambient temperature measured with an additional device  
T_Offset,Old: The old temperature offset.  

Since the CO2 sensor is actively heated, the temperature reading of the sensor is suposed
to be higher then the real ambient temperature. Therefore the temperature offset must be
a positive value in Kelvin.

Setting up the temperature offset is quite tricky. Maybe it is easier to introduce an offset
in your SmallBASIC program.

```
SCD30SetTemperatureOffset(2)
```

### SCD30SetCO2Recalibration
**Expert command**
```
SCD30SetCO2Recalibration(CO2ReferenceValue)
```
Set Forced Recalibration value (FRC).

Forced recalibration (FRC) is used to compensate for sensor drifts
when a reference value of the CO2 concentration in close proximity
to the SCD30 is available. For best results the sensor has to be
run in a stable environment in continuous mode at a measurement
rate of 2s for at least two minutes before applying the calibration
command and sending the reference value. Setting a reference CO2
concentration by the here described method will always overwrite
the settings from ASC and vice-versa. The reference CO2 concentration
has to be within the range 400 ppm ≤ cref(CO2) ≤ 2000 ppm. FRC value
is saved in non-volatile memory, the last set FRC value will be
used for field-calibration after repowering.

```
SCD30SetCO2Recalibration(1000)
```

### SCD30SetAltitudeRecalibration
**Expert command**
```
SCD30SetAltitudeRecalibration(Altitude)
```
Altitude >= 0 -> Height over sea level in m 

Altitude Compensation.

Measurements of CO2 concentration based on the NDIR principle are
influenced by altitude. SCD30 offers to compensate deviations due
to altitude by using the following command. Setting altitude is
disregarded when an ambient pressure is given

```
SCD30SetAltitudeRecalibration(1000)
```

### SCD30AutomaticSelfCalibration
**Expert command**
```
SCD30SCD30AutomaticSelfCalibration([TRUE or FALSE])
```
(De-)Activate Automatic Self-Calibration (ASC)

Continuous automatic self-calibration can be (de-)activated
with the following command. When activated for the first time a
period of minimum 7 days is needed so that the algorithm can find
its initial parameter set for ASC. The sensor has to be exposed
to fresh air for at least 1 hour every day. Also during that period,
the sensor may not be disconnected from the power supply, otherwise
the procedure to find calibration parameters is aborted and has to
be restarted from the beginning. The successfully calculated
parameters are stored in non-volatile memory of the SCD30 having
the effect that after a restart the previously found parameters
for ASC are still present. Note that the most recently found
self-calibration parameters will be actively used for self-
calibration disregarding the status of this feature. Finding a new
parameter set by the here described method will always overwrite
the settings from external recalibration and 
vice-versa. The feature is switched off by default. To work
properly SCD30 has to see fresh air on a regular basis.
Optimal working conditions are given when the sensor sees fresh
air for one hour every day so that ASC can constantly re-calibrate.
ASC only works in continuous measurement mode. ASC status is saved
in non-volatile memory. When the sensor is powered down while
ASC is activated SCD30 will continue with automatic self-calibration
after repowering without sending the command.

```
SCD30SCD30AutomaticSelfCalibration(1) 'Turns on ASC
SCD30SCD30AutomaticSelfCalibration(0) 'Turns off ASC
```

[back to main page](./index.html)
