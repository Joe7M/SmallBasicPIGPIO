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

#include "gpio.h"

int GPIO_SetPin(int argc, slib_par_t *params, var_t *retval)
{
	// Set the pin to be an input or output
	// Input = 0 
	// Output > 0
	// If input then pullup
	
	//get_param_int(argc,params, NumberOfParameter, DefaultValue)
	//      auto offset = get_param_int(argc, params, 1, 0);
	//String = get_param_str(argc, params, NumberOfParameter, nullptr);
	//     auto fileName = get_param_str(argc, params, 0, nullptr);
    
	int Pin    = get_param_int(argc, params, 0, 0);
	int InOut  = get_param_int(argc, params, 1, 1);
    
	if(InOut)
	{
		//Pin is an Output
		 gpioSetMode(Pin, PI_OUTPUT);		
	}
	else
	{
		//Pin is an Input
		 gpioSetMode(Pin, PI_INPUT);
		 gpioSetPullUpDown(Pin, PI_PUD_UP);
	}

	return(1);
		
}

int GPIO_Write(int argc, slib_par_t *params, var_t *retval)
{
	//Sets a Pin to high or low
	
	int Pin		= get_param_int(argc, params, 0, 0);
	int HighLow	= get_param_int(argc, params, 1, 0);
	
	gpioWrite(Pin, HighLow);
	
	return(1);
}

int GPIO_Read(int argc, slib_par_t *params, var_t *retval)
{
	//Read if Pin is high or low
	
	int Pin = get_param_int(argc, params, 0, 0);
	
	v_setint(retval, gpioRead(Pin));
	
	return(1);
}

int GPIO_Pwm(int argc, slib_par_t *params, var_t *retval)
{
	//Starts PWM on the given pin. The dutycycle is between 0 and 255
	// In case an LED is connected:
	// 0 		-> LED is off
	// 128 	-> LED has half brighness
	// 255 	-> LED has max. brighness
	
	int Pin			= get_param_int(argc, params, 0, 0);
	int Dutycycle 	= get_param_int(argc, params, 1, 0);
	
	gpioPWM(Pin, Dutycycle);
	
	return(1);
}

int GPIO_Trigger(int argc, slib_par_t *params, var_t *retval)
{
	//Emits a trigger pulse with a pulse length between 1 - 100 us
	//and a level high or low
	
	int Pin = get_param_int(argc, params, 0, 0);
	int PulseLength = get_param_int(argc, params, 1, 0);
	int Level = get_param_int(argc, params, 2, 0);
	
	gpioTrigger(Pin, PulseLength, Level);
	
	return(1);
	
}
