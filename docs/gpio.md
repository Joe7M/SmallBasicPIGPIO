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
If a _Pin_ is configured as output, the voltage _Level_ can be set to ground (low) or +3.3V (high).
```
GPIOWrite(4, 0)   'Sets pin 4 to low (ground)
GPIOWrite(4, 1)   'Sets pin 4 to high (+3.3V)
```

### GPIORead
```
Status = GPIORead(Pin)
```
If a _Pin_ is configured as input, the voltage _Level_ can be read. If the _Pin_ is at ground (low), _Status_ will be _0_. If the _Pin_ is at +3.3V (high) _Status_ will be 1.
```
Status = GPIOWRead(4)   'Reads the voltage level of pin 4
```

***
[back to main page](./index.html)
