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

#ifndef _BH1750_H_
#define _BH1750_H_

#include "var.h"
#include "param.h"

int BASIC_CMD_BH1750_SingleMeasurement(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_BH1750_Open(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_BH1750_Close(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_BH1750_SetResolution(int argc, slib_par_t *params, var_t *retval);

#endif /* !_BH1750_H_ */
