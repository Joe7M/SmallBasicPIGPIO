//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2021
//
//build:
//make
//or
//g++ -shared -fPIC -DPIC -O2 -nostdlib main.cpp gpio.c hashmap.cpp param.cpp  -o libSmallBasicPIGPIO.so -I./ -lpigpio
//
//SDL version	 : sudo sbasicg -m /home/pi/SmallBasicGPIO/bin/ -n lcd1.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ lcd1.bas

#ifndef _LCD1_H_
#define _LCD1_H_

#include "var.h"
#include "param.h"

bool bit_is_set(unsigned char b, int pos);
void lcd_send(unsigned char type, unsigned char c);
void lcd_write(const char *t);
int LCD1_Print(int argc, slib_par_t *params, var_t *retval);
int LCD1_Init(int argc, slib_par_t *params, var_t *retval);

#endif /* !_LCD1_H_ */ 
