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


#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <pigpio.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include <sched.h>

#include "gpio.h"
#include "ds18b20.h"

int DS18B20_Presence(int Pin)
{
	gpioSetMode(Pin, PI_OUTPUT);	
	gpioWrite(Pin, 0);
	gpioDelay(500);
	gpioSetMode(Pin, PI_INPUT);
	gpioDelay(70);
	int b = gpioRead(Pin);
	gpioDelay(410);
	return b;
}

void DS18B20_WriteBit(uint8_t Pin, int b)
{  
    int delay1, delay2;
    if (b == 1)
    {
        delay1 = 6;
        delay2 = 64;
    }
    else
    {
        delay1 = 80;
        delay2 = 10;
    }
    
    gpioSetMode(Pin, PI_OUTPUT);
    gpioWrite(Pin, 0);
    gpioDelay(delay1);
    gpioSetMode(Pin, PI_INPUT);
    gpioDelay(delay2);
}

void DS18B20_WriteByte(uint8_t Pin,int Byte)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (Byte & 1)
        {
            DS18B20_WriteBit(Pin,1);
        }
        else
        {
            DS18B20_WriteBit(Pin,0);
        }
        Byte = Byte >> 1;
    }
}

uint8_t DS18B20_ReadBit(uint8_t Pin)
{
    gpioSetMode(Pin, PI_OUTPUT);
    gpioWrite(Pin, 0);
    gpioDelay(6);
    gpioSetMode(Pin, PI_INPUT);
    gpioDelay(8);
    uint8_t b = gpioRead(Pin);
    gpioDelay(55);
    return b;
}

int DS18B20_ReadByte(uint8_t Pin)
{
    int byte = 0;
    int i;
    
    for (i = 0; i < 8; i++)
    {
        byte = byte | DS18B20_ReadBit(Pin) << i;
    };
    
    return byte;
}

uint8_t DS18B20_Crc8(uint8_t *Data, uint8_t Len)
{
    uint8_t i;
    uint8_t j;
    uint8_t Temp;
    uint8_t Databyte;
    uint8_t crc = 0;
 
    for (i = 0; i < Len; i++)
    {
        Databyte = Data[i];
        
        for (j = 0; j < 8; j++)
        {
            Temp = (crc ^ Databyte) & 0x01;
            crc >>= 1;
            if(Temp) crc ^= 0x8C;
            Databyte >>= 1;
        }
    }

    return crc;
}

int DS18B20_Convert(uint8_t Pin)
{
    int i;
    
    DS18B20_WriteByte(Pin, 0x44);
    
    for (i = 0; i < 1000; i++)
    {
        gpioDelay(800);
        if (DS18B20_ReadBit(Pin) == 1) break;
    }
    
    return i;
}


int DS18B20_GetTemperature(int argc, slib_par_t *params, var_t *retval)
{
	//Change scheduler to get linux in almost realtime mode.
	const struct sched_param priority = {1};
	sched_setscheduler(0, SCHED_FIFO, &priority);
	//mlockall(MCL_CURRENT | MCL_FUTURE);
    
	int Pin = get_param_int(argc, params, 0, 0);
    
	DS18B20_Presence(Pin);
	
	if (DS18B20_Presence(Pin) == 1) 
	{
		v_setreal(retval, -1000);
		return 1;
	}

	DS18B20_WriteByte(Pin, 0xCC);
	if(DS18B20_Convert(Pin) > 850)
	{
		v_setreal(retval, -3000);
		return 1;
	}

	DS18B20_Presence(Pin);
	DS18B20_WriteByte(Pin, 0xCC);
	DS18B20_WriteByte(Pin, 0xBE);

	int i;
	uint8_t Data[9];

	for(i = 0; i < 9; i++)
	{
	Data[i] = DS18B20_ReadByte(Pin);
	}

	uint8_t crc = DS18B20_Crc8(Data, 9);

	if(crc!=0)
	{
		v_setreal(retval, -2000);
		return 1;
	}

	int t1 = Data[0];
	int t2 = Data[1];

	int16_t Temp1 = (t2 << 8 | t1);
	float Temp = (float) Temp1 / 16;

	v_setreal(retval, Temp);

	//Change scheduler back to normal
	const struct sched_param priority_normal = {0};
	sched_setscheduler(0, SCHED_OTHER, &priority_normal);


	return(1);
}
