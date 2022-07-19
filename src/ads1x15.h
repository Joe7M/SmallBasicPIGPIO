/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT license
 * Joerg Siebenmorgen, 2022
*/

#ifndef ADS1X15_H
#define ADS1X15_H

#include "var.h"
#include "param.h"

int BASIC_CMD_ADS1X15_Open(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_Close(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_SetChannel(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_SetSampleRate(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_ReadVoltage(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_SetVoltageRange(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_SetContinuousMode(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_ADS1X15_SetSingleShotMode(int argc, slib_par_t *params, var_t *retval);

#endif

