#Basic GPIO Commands

###GPIOSetInput
```
GPIOSetInput(PinNumber)
```
Sets GPIO pin _PinNmuber_ as an input. The internal pullup-resistor will be set automaticly. 
Before using a GPIO pin, you should use either GPIOSetInput or GPIOSetOutput.

###GPIOSetOutput
```
GPIOSetOutput(PinNumber)
```
Sets GPIO pin _PinNmuber_ as an output. Before using a GPIO pin, you should use either GPIOSetInput or GPIOSetOutput.
