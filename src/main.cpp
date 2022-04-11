//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2022
//
//build:
//make
//or
//g++ -shared -fPIC -DPIC -O2 -nostdlib main.cpp gpio.c hashmap.cpp param.cpp  -o libSmallBasicPIGPIO.so -I./ -lpigpio
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <pigpio.h>
#include "gpio.h"
#include "ds18b20.h"
#include "lcd1.h"
#include "scd30.h"
#include "ssd1306.h"
#include "bh1750.h"

#include "var.h"
#include "module.h"
#include "param.h"



char *programSource = nullptr;

/*
static int cmd_textformat(int argc, slib_par_t *params, var_t *retval) {
  v_setstr(retval, format_text(argc, params, 0));
  return 1;
}

static int cmd_test(int argc, slib_par_t *params, var_t *retval) {
  v_setint(retval, 10);
  return 1;
}

*/

//Min Parameter , max Parameters, Function Name in Basic, Function in c 
FUNC_SIG lib_func[] =
{
  {1, 1, "GPIO_Read", GPIO_Read},
  {1, 1, "DS18B20_GetTemp", DS18B20_GetTemperature},

  {0, 0, "SCD30_DataAvailable", SCD30_BASIC_CMD_GetDataReadyStatus},
  {0, 0, "SCD30_ReadData", SCD30_BASIC_CMD_ReadMeasurement},
  {0, 4, "OLED1_CopyFromDisplay", BASIC_CMD_SSD1306_CopyFromDisplay},

  {0, 0, "BH1750_GetLight", BASIC_CMD_BH1750_SingleMeasurement}
};


static FUNC_SIG lib_proc[] =
{
  {1, 1, "GPIO_SetInput", GPIO_SetInput},
  {1, 1, "GPIO_SetOutput", GPIO_SetOutput},
  {2, 2, "GPIO_Write", GPIO_Write},
  {2, 2, "GPIO_Pwm", GPIO_Pwm},
  {3, 3, "GPIO_Trigger", GPIO_Trigger},

  {6, 6, "LCD1_Init", LCD1_Init},
  {1, 1, "LCD1_Print", LCD1_Print},
  {0, 0, "LCD1_Cls", LCD1_Cls},
  {2, 2, "LCD1_Locate", LCD1_Locate},
  {0, 0, "LCD1_Off", LCD1_Off},
  {0, 0, "LCD1_On", LCD1_On},

  {0, 0, "SCD30_Open", SCD30_BASIC_CMD_Open},
  {0, 0, "SCD30_Close", SCD30_BASIC_CMD_Close},
  {0, 1, "SCD30_Start", SCD30_BASIC_CMD_StartContMeasurement},
  {0, 0, "SCD30_Stop", SCD30_BASIC_CMD_StopContMeasurement},
  {1, 1, "SCD30_SetInterval", SCD30_BASIC_CMD_SetInterval},
  {1, 1, "SCD30_SetTemperatureOffset", SCD30_BASIC_CMD_SetTemperatureOffset},
  {1, 1, "SCD30_SetCO2Recalibration", SCD30_BASIC_CMD_CO2Recalibration},
  {1, 1, "SCD30_SetAltitudeCompensation", SCD30_BASIC_CMD_AltitudeCompensation},
  {1, 1, "SCD30_AutomaticSelfCalibration", SCD30_BASIC_CMD_AutomaticSelfCalibration},

  {0, 3, "OLED1_Open", BASIC_CMD_SSD1306_Open},
  {0, 0, "OLED1_Close", BASIC_CMD_SSD1306_Close},
  {0, 0, "OLED1_Display", BASIC_CMD_SSD1306_Display},
  {0, 1, "OLED1_Cls", BASIC_CMD_SSD1306_ClearDisplay},
  {2, 3, "OLED1_Pset", BASIC_CMD_SSD1306_Pset},
  {4, 5, "OLED1_Line", BASIC_CMD_SSD1306_Line},
  {4, 6, "OLED1_Rect", BASIC_CMD_SSD1306_Rect},
  {4, 7, "OLED1_RoundRect", BASIC_CMD_SSD1306_RoundRect},
  {3, 5, "OLED1_Circle", BASIC_CMD_SSD1306_Circle},
  {6, 8, "OLED1_Triangle", BASIC_CMD_SSD1306_Triangle},
  {0, 2, "OLED1_Print", BASIC_CMD_SSD1306_Print},
  {1, 4, "OLED1_CopyToDisplay", BASIC_CMD_SSD1306_CopyToDisplay},
  {1, 1, "OLED1_SetTextSize", BASIC_CMD_SSD1306_SetTextSize},
  {2, 2, "OLED1_At", BASIC_CMD_SSD1306_At},
  {0, 1, "OLED1_SetBrightness", BASIC_CMD_SSD1306_SetBrightness},
  {0, 0, "OLED1_InvertDisplay", BASIC_CMD_SSD1306_InvertDisplay},

  {0, 2, "BH1750_Open", BASIC_CMD_BH1750_Open},
  {0, 0, "BH1750_Close", BASIC_CMD_BH1750_Close},
  {0, 1, "BH1750_SetResolution", BASIC_CMD_BH1750_SetResolution}
};


SBLIB_API int sblib_proc_count()
{
  return (sizeof(lib_proc) / sizeof(lib_proc[0]));
}

int sblib_func_count()
{
  return (sizeof(lib_func) / sizeof(lib_func[0]));
}

SBLIB_API int sblib_proc_getname(int index, char *proc_name)
{
  int result;
  if (index < sblib_proc_count()) {
    strcpy(proc_name, lib_proc[index]._name);
    result = 1;
  } else {
    result = 0;
  }
  return result;
}



int sblib_func_getname(int index, char *proc_name)
{
  int result;
  if (index < sblib_func_count()) {
    strcpy(proc_name, lib_func[index]._name);
    result = 1;
  } else {
    result = 0;
  }
  return result;
}

SBLIB_API int sblib_proc_exec(int index, int argc, slib_par_t *params, var_t *retval) {
  int result;
  if (index >= 0 && index < sblib_proc_count()) {
    if (argc < lib_proc[index]._min || argc > lib_proc[index]._max) {
      if (lib_proc[index]._min == lib_proc[index]._max) {
        error(retval, lib_proc[index]._name, lib_proc[index]._min);
      } else {
        error(retval, lib_proc[index]._name, lib_proc[index]._min, lib_proc[index]._max);
      }
      result = 0;
    } else {
      result = lib_proc[index]._command(argc, params, retval);
    }
  } else {
    fprintf(stderr, "SmallBasicPIGPIO: PROC index error [%d]\n", index);
    result = 0;
  }
  return result;
}



SBLIB_API int sblib_func_exec(int index, int argc, slib_par_t *params, var_t *retval) {
  int result;
  if (index >= 0 && index < sblib_func_count()) {
    if (argc < lib_func[index]._min || argc > lib_func[index]._max) {
      if (lib_func[index]._min == lib_func[index]._max) {
        error(retval, lib_func[index]._name, lib_func[index]._min);
      } else {
        error(retval, lib_func[index]._name, lib_func[index]._min, lib_func[index]._max);
      }
      result = 0;
    } else {
      result = lib_func[index]._command(argc, params, retval);
    }
  } else {
    fprintf(stderr, "SmallBasicPIGPIO: FUNC index error [%d]\n", index);
    result = 0;
  }
  return result;
}

int sblib_init(const char *sourceFile) {
  programSource = strdup(sourceFile);
  
  //Init GPIO when plugin is loaded
  if ( gpioInitialise() < 0 ) return(0);
  
  return 1;
}

void sblib_close(void)
{
  free(programSource);
  
  //end gpio when plugin is closed
  gpioTerminate();
}
