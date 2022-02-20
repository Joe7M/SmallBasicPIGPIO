 
//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2021

#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "var.h"
#include "param.h"


int BASIC_CMD_SSD1306_Open(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Close(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Display(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_ClearDisplay(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Pset(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Line(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Rect(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Circle(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_RoundRect(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Triangle(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_Print(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_CopyToDisplay(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_SetTextSize(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_At(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_SetBrightness(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_InvertDisplay(int argc, slib_par_t *params, var_t *retval);
int BASIC_CMD_SSD1306_CopyFromDisplay(int argc, slib_par_t *params, var_t *retval);

#endif /* !_SSD1306_H_ */
