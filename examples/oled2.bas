' SmallBASIC
'
' Example on how to use a SSD1306 OLED Display
' A png image is loaded from hard drive, converted to an array and
' displayed on the oled
'
' Tested with SmallBASIC 12.23
' By Joerg Siebenmorgen
' MIT Licence, 2022
'
' You have to use the SDL-Version, because image.save(Array) is not
' supported in console version:
' SDL version: sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -n oled2.bas
'

import SmallBasicPIGPIO as gpio

'Load an image
I = image("oled1_example.png")

'Convert image to array with gray values
A = PngToGrayArray(I)

'Display array on the OLED
gpio.OLED1_Open()

gpio.OLED1_Cls()
gpio.OLED1_CopyToDisplay(A,0,0,3)  'using mode 3 for best transparency
gpio.OLED1_Display()

delay(2000)
gpio.OLED1_Close()


'Function for converting an image to an array with gray values
func PngToGrayArray(byref Img)

    local A, A_Rotated, r, g, b, xx, yy
    dim A

    Img.save(A)

    dim A_Rotated(Img.width, Img.height)

    for xx = 0 to Img.width - 1
        for yy = 0 to Img.height - 1

            'alpha = (A[xx,yy] & 0xFF000000) rshift 24
            r     = ((A[xx,yy] & 0x00FF0000) rshift 16)
            g     = ((A[xx,yy] & 0x0000FF00) rshift 8)
            b     = ((A[xx,yy] & 0x000000FF))

            ' this is a quite bad conversion to gray values but will do for the oled.
            A_Rotated[yy,xx] = ceil((r+g+b) / 3)
        next
    next

    PngToGrayArray = A_Rotated

end func





