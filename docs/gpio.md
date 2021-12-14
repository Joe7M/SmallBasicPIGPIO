[back to main page](./index.html)

# Basic GPIO Commands

### GPIOSetInput
```
GPIOSetInput(PinNumber)
```
Sets GPIO pin _PinNumber_ as an input. The internal pullup-resistor will be set automaticly. 
Before using a GPIO pin, you should use either GPIOSetInput or GPIOSetOutput.

### GPIOSetOutput
```
GPIOSetOutput(PinNumber)
```
Sets GPIO pin _PinNumber_ as an output. Before using a GPIO pin, you should use either GPIOSetInput or GPIOSetOutput.

### GPIOWrite
```
GPIOWrite(Pin, Level)
```
If _Pin_ is configured as output, the voltage _Level_ can be set to ground (low) or +3.3V (high).
```
GPIOWrite(4, 0)   'Sets pin 4 to low (ground)
GPIOWrite(4, 1)   'Sets pin 4 to high (+3.3V)
```

### GPIORead
```
Status = GPIORead(Pin)
```
If _Pin_ is configured as input, the voltage _Level_ can be read. If the _Pin_ is at ground (low), _Status_ will be _1_. If the _Pin_ is at +3.3V (high) _Status_ will be 0.
```
Status = GPIOWRead(4)   'Reads the voltage level of pin 4
```

### GPIOTrigger
```
GPIOTrigger(Pin, Duration, Level)
```
If _Pin_ is configured as output, a trigger pulse at _Pin_ will be emited with a _Duration_ in microseconds and a _Level_ of low or high.
```
GPIOTrigger(4, 50, 1)  'A 50 µs Trigger pulse at pin 4 with +3.3V (high)
```

### GPIOPwm
```
GPIOPwm(Pin, PWMLevel)
```
If _Pin_ is configured as output, a PWM-signal (Pules Width Modulation) will be emitted to _Pin_ with a duty cycle of _PWMLevel_. _PWMLevel_ can be a value from 0 to 255.
```
GPIOPwm(4, 128)  'Sets PWM with a duty cycle of 50% to pin 4
```

***
[back to main page](./index.html)
