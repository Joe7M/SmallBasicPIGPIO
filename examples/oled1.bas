' SmallBASIC
'
' Example on how to use a SSD1306 OLED Display
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2022
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin oled1.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -n oled1.bas
'

import SmallBasicPIGPIO as gpio

gpio.OLED1_Open()

gpio.OLED1_SetBrightness(128)

gpio.OLED1_Cls()

gpio.OLED1_Pset(10,0)
gpio.OLED1_Line(0,0,127,63)
gpio.OLED1_RoundRect(54,26,74,38,5)
gpio.OLED1_Circle(118,10,5,1,1) 'Filled with white
gpio.OLED1_Triangle(118,30, 113,45, 123,45, 1, 1) 'Filled with white

gpio.OLED1_At(0,56)
gpio.OLED1_SetTextSize(16)
gpio.OLED1_Print("SmallBASIC")


'Let's move a image of a smiley with transparency over the screen

'Create an image array with 16x16 Pixels
dim SmilyGuyArray(15,15)

for yy = 0 to 15
  for xx = 0 to 15
    read d
    SmilyGuyArray(xx,yy) = d
  next
next

dim Background

for xx = 1 to 127

    Background = gpio.OLED1_CopyFromDisplay(xx, 25, 16, 16) 'Save the part of the OLED display,
                                                            'where we want to draw the smiley.
    gpio.OLED1_CopyToDisplay(SmilyGuyArray, xx, 25, 3)      'Copy the array to the display
                                                            'Mode = 3 -> Pixel value: 0=Black;
                                                            '255=White; Every other number is
                                                            'rendered transparent
    gpio.OLED1_Display()                                    'Display the buffer
    gpio.OLED1_CopyToDisplay(Background, xx, 25)            'Copy the saved background back to
                                                            'the display. This will remove the
                                                            'smiley. In the next iteration of
                                                            'the loop, you have the original
                                                            'background image.
next

delay(2000)
gpio.OLED1_Close()

print("Done")


'Smiley Guy
'0=Black; 255=White; Every other number is transparent
DATA   1,  1,  1,255,255,255,255,255,255,255,255,255,255,  1,  1,  1
DATA   1,  1,255,255,255,255,255,255,255,255,255,255,255,255,  1,  1
DATA   1,  1,255,255,255,255,255,255,255,255,255,255,255,255,  1,  1
DATA   1,  1,255,255,  0,  0,  0,  0,  0,  0,  0,  0,255,255,  1,  1
DATA   1,  1,255,  0,255,255,255,  0,  0,255,255,255,  0,255,  1,  1
DATA   1,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  ,0  ,255,1
DATA 255,255,  0,  0,255,255,  0,  0,  0,  0,255,255,  0,  0,255,255
DATA 255,255,  0,  0,255,255,  0,255,  0,  0,255,255,  0,  0,255,255
DATA 255,255,  0,  0,  0,  0,  0,255,255,  0,  0,  0,  0,  0,255,255
DATA   1,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  1
DATA   1,255,  0,  0,255,  0,  0,  0,  0,  0,  0,255,  0,  0,255,  1
DATA   1,  1,255,  0,  0,255,  0,  0,  0,  0,255,  0,  0,255,  1,  1
DATA   1,  1,255,  0,  0,  0,255,255,255,255,  0,  0,  0,255,  1,  1
DATA   1,  1,  1,255,  0,  0,  0,  0,  0,  0,  0,  0,255,  1,  1,  1
DATA   1,  1,  1,  1,255,255,255,255,255,255,255,255,  1,  1,  1,  1
DATA   1,  1,  1,  1,  1,255,255,255,255,255,255,  1,  1,  1,  1,  1


