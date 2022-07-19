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
//SDL version	 : sudo sbasicg -m /home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#ifndef _GPIO_H_
#define _GPIO_H_

#include "var.h"
#include "param.h"

int GPIO_Initialise(int argc, slib_par_t *params, var_t *retval);
int GPIO_Terminate(int argc, slib_par_t *params, var_t *retval);
int GPIO_SetInput(int argc, slib_par_t *params, var_t *retval);
int GPIO_SetOutput(int argc, slib_par_t *params, var_t *retval);
int GPIO_Write(int argc, slib_par_t *params, var_t *retval);
int GPIO_Read(int argc, slib_par_t *params, var_t *retval);
int GPIO_Pwm(int argc, slib_par_t *params, var_t *retval);
int GPIO_Trigger(int argc, slib_par_t *params, var_t *retval);

#endif /* !_GPIO_H_ */ 
