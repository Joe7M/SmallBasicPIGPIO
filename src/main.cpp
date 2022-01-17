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
#include "gpio.h"
#include "ds18b20.h"
#include "lcd1.h"
#include "scd30.h"

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
  {1, 1, "GPIORead", GPIO_Read},
  {1, 1, "DS18B20Temp", DS18B20_GetTemperature},

  {0, 0, "SCD30DataAvailable", SCD30_BASIC_CMD_GetDataReadyStatus},
  {0, 0, "SCD30ReadData", SCD30_BASIC_CMD_ReadMeasurement}
};


static FUNC_SIG lib_proc[] =
{
  {1, 1, "GPIOSetInput", GPIO_SetInput},
  {1, 1, "GPIOSetOutput", GPIO_SetOutput},
  {2, 2, "GPIOWrite", GPIO_Write},
  {2, 2, "GPIOPwm", GPIO_Pwm},
  {3, 3, "GPIOTrigger", GPIO_Trigger},

  {6, 6, "LCD1Init", LCD1_Init},
  {1, 1, "LCD1Print", LCD1_Print},
  {0, 0, "LCD1Cls", LCD1_Cls},
  {2, 2, "LCD1Locate", LCD1_Locate},
  {0, 0, "LCD1Off", LCD1_Off},
  {0, 0, "LCD1On", LCD1_On},

  {0, 0, "SCD30Open", SCD30_BASIC_CMD_Open},
  {0, 0, "SCD30Close", SCD30_BASIC_CMD_Close},
  {0, 1, "SCD30Start", SCD30_BASIC_CMD_StartContMeasurement},
  {0, 0, "SCD30Stop", SCD30_BASIC_CMD_StopContMeasurement},
  {1, 1, "SCD30SetInterval", SCD30_BASIC_CMD_SetInterval},
  {1, 1, "SCD30SetTemperatureOffset", SCD30_BASIC_CMD_SetTemperatureOffset},
  {1, 1, "SCD30SetCO2Recalibration", SCD30_BASIC_CMD_CO2Recalibration},
  {1, 1, "SCD30SetAltitudeCompensation", SCD30_BASIC_CMD_AltitudeCompensation},
  {1, 1, "SCD30AutomaticSelfCalibration", SCD30_BASIC_CMD_AutomaticSelfCalibration}

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
