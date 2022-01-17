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

#ifndef _SCD30_H_
#define _SCD30_H_

#include "var.h"
#include "param.h"

#define SCD30_ADDRESS 0x61

int SCD30_BASIC_CMD_StartContMeasurement(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_StopContMeasurement(int argc, slib_par_t *params, var_t *retval);;
int SCD30_BASIC_CMD_SetInterval(int argc, slib_par_t *params, var_t *retval);;
int SCD30_BASIC_CMD_GetDataReadyStatus(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_ReadMeasurement(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_SetTemperatureOffset(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_CO2Recalibration(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_AltitudeCompensation(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_Open(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_Close(int argc, slib_par_t *params, var_t *retval);
int SCD30_BASIC_CMD_AutomaticSelfCalibration(int argc, slib_par_t *params, var_t *retval);


#endif /* !_SCD30_H_ */
