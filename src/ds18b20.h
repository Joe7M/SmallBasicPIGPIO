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

#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "var.h"
#include "param.h"

int DS18B20_Presence(uint8_t Pin);
void DS18B20_WriteBit(uint8_t Pin, int b);
void DS18B20_WriteByte(uint8_t Pin,int Byte);
uint8_t DS18B20_ReadBit(uint8_t Pin);
int DS18B20_ReadByte(uint8_t Pin);
uint8_t DS18B20_Crc8(uint8_t *Data, uint8_t Len);
int DS18B20_Convert(uint8_t Pin);
int DS18B20_GetTemperature(int argc, slib_par_t *params, var_t *retval);


#endif /* !_DS18B20_H_ */ 
