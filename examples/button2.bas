' Extended example for using buttons with the SmallBASIC PiGPIO plugin
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2022
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin buttons.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r buttons.bas
'
' Connect two push buttons to the Raspberry Pi to the pins 20 and 26.
' The example demonstrates the creation of a button object and testing if
' the button was pressed. One button will use the ButtonPressed function,
' which will return 1 if the button is pressed. The other button will use
' the ButtonPressedAgain function. It will return 1, if the button state
' changed from not-pressed to pressed. If you keep the button pressed
' wihtout releasing it, this function will return 0.


import SmallBasicPIGPIO as gpio

Button_1 = CreatePushButton(20)
Button_2 = CreatePushButton(26)

print("Press the buttons and see what happends")

while(1)    
	if(ButtonPressed(Button_1)) then
	    print("Button 1")
	endif
	if(ButtonPressedAgain(Button_2)) then
	    print("Button 2")
	endif
                
    delay(0.1)
wend

'######################################################################

func CreatePushButton(Pin)
    local B
    B.Pin = Pin
    B.DebounceTimer = ticks()
    B.LastState = false
    
    gpio.GPIO_SetInput(Pin)
    
    CreatePushButton = B
end func


func ButtonPressed(byref B)
    
    if(ticks() - B.DebounceTimer > 10) then
        state = 1 - gpio.GPIO_READ(B.Pin)
        B.DebounceTimer = ticks()
        if(B.LastState != state) then
            B.DebounceTimer = 0
            B.LastState = state
        end if
    else
        state = B.LastState
    end if
    
    ButtonPressed = state
    
end func

func ButtonPressedAgain(byref B)
    local ReturnVal

    ReturnVal = 0
    
    if(ticks() - B.DebounceTimer > 10) then
        state = 1 - gpio.GPIO_READ(B.Pin)
        B.DebounceTimer = ticks()                     
        if(B.LastState != state) then
            B.DebounceTimer = 0
            B.LastState = state
            ReturnVal = state            
        end if        
    end if
    
    ButtonPressedAgain = ReturnVal
    
end func



