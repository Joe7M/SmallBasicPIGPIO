/*This file is part of the SmallBasicPIGPIO plugin to
use gpio pins on a Raspberry pi with SmallBASIC.
MIT licence
Joerg Siebenmorgen, 2021

build:
make
or
g++ -shared -fPIC -DPIC -O2 -nostdlib main.cpp gpio.c hashmap.cpp param.cpp  -o libSmallBasicPIGPIO.so -I./ -lpigpio

SDL version	 : sudo sbasicg -m /home/pi/SmallBasicGPIO/bin/ -n lcd1.bas
Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ lcd1.bas

Code:
https://www.ne555.at/2014/index.php/atmel-avr-mikrocontrollertechnik/382-lcd-display-ansteuern.html
https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
*/


#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <pigpio.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "lcd1.h"

int Pin_RS = 5;
int Pin_E = 6;
int Pin_D4 = 13;
int Pin_D5 = 19;
int Pin_D6 = 26;
int Pin_D7 = 12;

#define LCD_RAM_ADRESS 		   0x80
#define LCD_CLEAR				0x01
#define LCD_HOME				0x02
#define LCD_ON_CURSOR_OFF		0x0C
#define LCD_ON_CURSOR_ON		0x0E
#define LCD_OFF					0x08

#define COMMAND				0
#define DATA					1

//Helper function to test, if a bit is set
//https://stackoverflow.com/questions/2431732/checking-if-a-bit-is-set-or-not
bool bit_is_set(unsigned char b, int pos)
{
   return (b & (1 << pos)) != 0;
}

// Send 1 Byte
void lcd_send(unsigned char type, unsigned char c)
{
	if (type == COMMAND)
		gpioWrite(Pin_RS, 0);
	else
		gpioWrite(Pin_RS, 1);

	//LCD is used in 4bit mode
	//Send high bits first
	if (bit_is_set(c, 7))
		gpioWrite(Pin_D7,1); 
	else
		gpioWrite(Pin_D7,0);
	
	if (bit_is_set(c, 6))
		gpioWrite(Pin_D6,1); 
	else
		gpioWrite(Pin_D6,0);
	
	if (bit_is_set(c, 5))
		gpioWrite(Pin_D5,1); 
	else
		gpioWrite(Pin_D5,0);
	
	if (bit_is_set(c, 4))
		gpioWrite(Pin_D4,1); 
	else
		gpioWrite(Pin_D4,0);
	
	// Pins are set, tell lcd to read the pins
	gpioWrite(Pin_E, 1);
	gpioDelay(1000); 	//1ms
	gpioWrite(Pin_E, 0);
    gpioDelay(1000); 	//1ms

	//Transfor lower bits
	if (bit_is_set(c, 3))
		gpioWrite(Pin_D7,1); 
	else
		gpioWrite(Pin_D7,0);
	
	if (bit_is_set(c, 2))
		gpioWrite(Pin_D6,1); 
	else
		gpioWrite(Pin_D6,0);
	
	if (bit_is_set(c, 1))
		gpioWrite(Pin_D5,1); 
	else
		gpioWrite(Pin_D5,0);
	
	if (bit_is_set(c, 0))
		gpioWrite(Pin_D4,1); 
	else
		gpioWrite(Pin_D4,0);
	
	 // Pins are set, tell lcd to read the pins
	gpioWrite(Pin_E, 1);
	gpioDelay(1000); 	//1ms
	gpioWrite(Pin_E, 0);

	gpioDelay(5000);
}

//Write characters to lcd
void lcd_write(const char *t)
{
	unsigned char i;
	for (i = 0; i < 255; i++)
	{
		if (t[i] == 0) return; //String and found
		lcd_send(DATA, t[i]);
	}
}

int LCD1_Init(int argc, slib_par_t *params, var_t *retval)
{
	// Init the LCD
	    
	Pin_RS	= get_param_int(argc, params, 0, 5);
	Pin_E	= get_param_int(argc, params, 1, 6);
	Pin_D4	= get_param_int(argc, params, 2, 13);
	Pin_D5	= get_param_int(argc, params, 3, 19);
	Pin_D6	= get_param_int(argc, params, 4, 26);
	Pin_D7	= get_param_int(argc, params, 5, 12);
	
	gpioSetMode(Pin_RS, PI_OUTPUT);
	gpioSetMode(Pin_E, PI_OUTPUT);
	gpioSetMode(Pin_D4, PI_OUTPUT);
	gpioSetMode(Pin_D5, PI_OUTPUT);
	gpioSetMode(Pin_D6, PI_OUTPUT);
	gpioSetMode(Pin_D7, PI_OUTPUT);
	
	gpioDelay(40000);

    // reset lcd
    lcd_send(COMMAND, 0x33);
    //lcd_send(COMMAND, 0x32);



	// Set 4-bit mode -> Function set 1
	gpioWrite(Pin_D5, 1);
	gpioWrite(Pin_D4, 0);
	gpioDelay(40000);
	// configure 4-bit mode -> Function set 2
	gpioWrite(Pin_D5, 1);
	gpioWrite(Pin_D4, 0);
	gpioDelay(5000);
	// configure 4-bit mode -> Function set 3
	gpioWrite(Pin_D5, 1);
	gpioWrite(Pin_D4, 0);
	// start 4-bit mode
	gpioWrite(Pin_E, 1);
	gpioWrite(Pin_E, 0);
	gpioDelay(5000); 

	// LCD with 2 rows in 4-bit mode
	lcd_send(COMMAND, 0x28);
	//lcd_send(COMMAND, 0x14);  // 1 row

	lcd_send(COMMAND, LCD_OFF);
	lcd_send(COMMAND, LCD_CLEAR);
	lcd_send(COMMAND, 0x06);
	lcd_send(COMMAND, LCD_ON_CURSOR_OFF);
	
	return(1);
}

int LCD1_Print(int argc, slib_par_t *params, var_t *retval)
{
	auto Text = get_param_str(argc, params, 0, nullptr);
	
	lcd_write(Text);
	
	return(1);
}

int LCD1_Cls(int argc, slib_par_t *params, var_t *retval)
{
    lcd_send(COMMAND, LCD_CLEAR);
    lcd_send(COMMAND, LCD_HOME);

    return(1);
}

int LCD1_Off(int argc, slib_par_t *params, var_t *retval)
{
    lcd_send(COMMAND, LCD_OFF);

    return(1);
}

int LCD1_On(int argc, slib_par_t *params, var_t *retval)
{
    lcd_send(COMMAND, LCD_ON_CURSOR_OFF);

    return(1);
}


int LCD1_Locate(int argc, slib_par_t *params, var_t *retval)
{
    int data;
    int x	= get_param_int(argc, params, 0, 0);
	int y	= get_param_int(argc, params, 1, 0);


    switch (y)
    {
        case 1:    // 1. Zeile
            data = LCD_RAM_ADRESS + 0x00 + x;
            break;

        case 2:    // 2. Zeile
            data = LCD_RAM_ADRESS + 0x40 + x;
            break;

        case 3:    // 3. Zeile
            data = LCD_RAM_ADRESS + 0x10 + x;
            break;

        case 4:    // 4. Zeile
            data = LCD_RAM_ADRESS + 0x50 + x;
            break;

        default:
            return(0);                                   // für den Fall einer falschen Zeile
    }

    lcd_send(COMMAND, data );

    return(1);
}
