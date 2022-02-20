[back to main page](./index.html)

# Basic GPIO Commands

### GPIO_SetInput
```
GPIO_SetInput(PinNumber)
```
PinNumber = 0 ... 53

Sets GPIO pin _PinNumber_ as an input. The internal pullup-resistor will be set automaticly. 
Before using a GPIO pin, you should use either GPIOSetInput or GPIOSetOutput.

### GPIO_SetOutput
```
GPIO_SetOutput(PinNumber)
```
PinNumber = 0 ... 53

Sets GPIO pin _PinNumber_ as an output. Before using a GPIO pin, you should use either GPIOSetInput or GPIOSetOutput.

### GPIO_Write
```
GPIO_Write(Pin, Level)
```
Pin = 0 ... 53  
Level = 0 or 1

If _Pin_ is configured as output, the voltage _Level_ can be set to ground (low) or +3.3V (high).
```
GPIO_Write(4, 0)   'Sets pin 4 to low (ground)
GPIO_Write(4, 1)   'Sets pin 4 to high (+3.3V)
```

### GPIO_Read
```
Status = GPIO_Read(Pin)
```
Pin = 0 ... 53  
Status = 0 or 1

If _Pin_ is configured as input, the voltage _Level_ can be read. If the _Pin_ is at ground (low), _Status_ will be _1_. If the _Pin_ is at +3.3V (high) _Status_ will be 0.
```
Status = GPIO_Read(4)   'Reads the voltage level of pin 4
```

### GPIO_Trigger
```
GPIO_Trigger(Pin, Duration, Level)
```
Pin = 0 ... 53  
Duration = 1 ... 100  
Level = 0 or 1

If _Pin_ is configured as output, a trigger pulse at _Pin_ will be emitted with a _Duration_ in microseconds and a _Level_ of low or high.
```
GPIO_Trigger(4, 50, 1)  'A 50 µs Trigger pulse at pin 4 with +3.3V (high)
```

### GPIOPwm
```
GPIO_Pwm(Pin, PWMLevel)
```
Pin = 0 ... 53  
PWMLevel = 0 ... 255

If _Pin_ is configured as output, a PWM-signal (Pules Width Modulation) will be emitted to _Pin_ with a duty cycle of _PWMLevel_. _PWMLevel_ can be a value from 0 to 255.
```
GPIO_Pwm(4, 128)  'Sets PWM with a duty cycle of 50% to pin 4
```

***
[back to main page](./index.html)
