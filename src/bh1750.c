//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2021
//
//build:
//make
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin/ -r scd30.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin/ scd30.bas

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <pigpio.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "bh1750.h"

#define RESOLUTION_LOW 0
#define RESOLUTION_HIGH 1


int bh1750;
int resolution;

int BASIC_CMD_BH1750_SingleMeasurement(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[1];
    char output_buffer[2];
    uint16_t result;

    //Power on
    input_buffer[0] = 0b1;
    i2cWriteDevice(bh1750, input_buffer, 1);

    if(resolution == RESOLUTION_HIGH)
    {
        //Start single H-resolution measurement
        input_buffer[0] = 0b100000;
        i2cWriteDevice(bh1750, input_buffer, 1);
        gpioDelay(180*1000);
    }
    else
    {
        //Start single L-resolution measurement
        input_buffer[0] = 0b100011;
        i2cWriteDevice(bh1750, input_buffer, 1);
        gpioDelay(24*1000);
    }

    //Read result
    i2cReadDevice(bh1750, output_buffer, 2);
    result = ((output_buffer[0] << 8) + output_buffer[1]) / 1.2;
    v_setint(retval, result);

    return 1;

}

int BASIC_CMD_BH1750_SetResolution(int argc, slib_par_t *params, var_t *retval)
{
    int res = get_param_int(argc, params, 0, 0);

    if(res == 0)
        resolution = RESOLUTION_LOW;
    else
        resolution = RESOLUTION_HIGH;

    return 1;

}


int BASIC_CMD_BH1750_Open(int argc, slib_par_t *params, var_t *retval)
{
	int bh1750_address = get_param_int(argc, params, 0, 0x23);
    int res = get_param_int(argc, params, 1, 1);

    if(res == 0)
        resolution = RESOLUTION_LOW;
    else
        resolution = RESOLUTION_HIGH;

    bh1750 = i2cOpen(1, bh1750_address, 0);

    if(bh1750 < 0)
    {
        v_setstr(retval, "Cannot connect to sensor.");
        return (0);
    }

    return(1);
}

int BASIC_CMD_BH1750_Close(int argc, slib_par_t *params, var_t *retval)
{
	i2cClose(bh1750);

	return(1);
}
