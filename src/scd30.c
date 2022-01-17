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

#include <sched.h>

#include "gpio.h"
#include "scd30.h"

int scd30;

/* I2C Checksum calculation
 * Name:  CRC-8
 * Protected Data: read data
 * Width: 8 bits
 * Polynomial: 0x31 (x8 + x5 + x4 + 1)
 * Initialization: 0xFF
 * Reflect Input: false
 * Reflect Output: false
 * Final XOR: 0x00
 * Example: CRC(0xBEEF) = 0x92
 *
 * https://stackoverflow.com/questions/51752284/how-to-calculate-crc8-in-c
 */
char gencrc(char *data)
{
    char crc = 0xff;
    size_t i, j;
    for (i = 2; i < 4; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            if ((crc & 0x80) != 0) crc = (char)((crc << 1) ^ 0x31);
            else crc <<= 1;
        }
    }
    return crc;
}

/* Trigger continuous measurement with optional ambient pressure compensation
 * 0x0010 + Pressure in mBar 0,700..1200
 * The CO2 measurement value can be compensated for ambient pressure
 * by feeding the pressure value in mBar to the sensor.
 * Setting the ambient pressure will overwrite previous and
 * future settings of altitude compensation. Setting the argument
 * to zero will deactivate the ambient pressure compensation.
 * For setting a new ambient pressure when continuous measurement is
 * running the whole command has to be written to SCD30
 * Pressure = 0 deactivates pressure compensation.
 */
int SCD30_BASIC_CMD_StartContMeasurement(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[5] = {0};

    int Pressure = get_param_int(argc, params, 0, 0);

    if(Pressure == 0 || (Pressure > 699 && Pressure < 1201))
    {
        input_buffer[0] = (0x0010 >> 8) & 0xff;
        input_buffer[1] = (0x0010) & 0xff;
        input_buffer[2] = (Pressure >> 8) & 0xff;
        input_buffer[3] = (Pressure) & 0xff;
        input_buffer[4] = gencrc(input_buffer);


        i2cWriteDevice(scd30, input_buffer, 5);

        return 1;
    }
    else
    {
        v_setstr(retval, "Invalid air pressure (0, 700..1200)");
        return 0;
    }
}

/* Stop continuous measurement
 * 0x0104
 * Stops the continuous measurement
 */

int SCD30_BASIC_CMD_StopContMeasurement(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[2] = {0};

    input_buffer[0] = (0x0104 >> 8) & 0xff;
	input_buffer[1] = (0x0104) & 0xff;

	i2cWriteDevice(scd30, input_buffer, 2);

    return 1;
}

/* Set measurement interval
 * 0x4600 + Interval 2...1800s
 * Sets the interval used by the SCD30 sensor to measure in continuous
 * measurement mode. Initial value is 2 s.
 */
int SCD30_BASIC_CMD_SetInterval(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[5] = {0};

    int Interval = get_param_int(argc, params, 0, 0);

    if(Interval < 2 || Interval > 1800)
    {
        v_setstr(retval, "Invalid interval (2...1800s)");
        return 0;
    }

    input_buffer[0] = (0x4600 >> 8) & 0xff;
	input_buffer[1] = (0x4600) & 0xff;
    input_buffer[2] = (Interval >> 8) & 0xff;
	input_buffer[3] = (Interval) & 0xff;
    input_buffer[4] = gencrc(input_buffer);

	i2cWriteDevice(scd30, input_buffer, 5);

    return 1;
}

/* Get data ready status
 * Send 0x0202
 * Recieve: 0 or 1
 * Data ready command is used to determine if a measurement can
 * be read from the sensor’s buffer. Whenever there is a measurement
 * available from the internal buffer this command returns 1 and 0
 * otherwise. As soon as the measurement has been read by the return
 * value changes to 0.
 */
int SCD30_BASIC_CMD_GetDataReadyStatus(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[2] = {0};
    char output_buffer[2] = {0};

    input_buffer[0] = (0x0202 >> 8) & 0xff;
	input_buffer[1] = (0x0202) & 0xff;

    i2cWriteDevice(scd30, input_buffer, 2);
    i2cReadDevice(scd30, output_buffer, 2);

    v_setreal(retval, output_buffer[1]);

    return 1;
}

/* Read measurement
 * send 0x0300
 * reviece 18 bytes: 1+2:   CO2;         3:  CRC;   4+5: CO2;          6: CRC
 *                   7+8:   Temperature; 9:  CRC; 10+11: Temperature; 12: CRC
 *                   13+14: Humidity;    15: CRC; 16+17: Humidity;    18: CRC
 * When new measurement data is available it can be read out with the following
 * command. Make sure that the measurement is completed by reading the data ready
 * status bit before read out.
 */
int SCD30_BASIC_CMD_ReadMeasurement(int argc, slib_par_t *params, var_t *retval)
{
    uint32_t co2 = 0;
	uint32_t temperature = 0;
	uint32_t humidity = 0;
    float co2_float = 0;
    float temperature_float = 0;
    float humidity_float = 0;

	char input_buffer[2] = {0};
    char output_buffer[18] = {0};

    var_t *v;
    var_t *p;

    input_buffer[0] = (0x0300 >> 8) & 0xff;
	input_buffer[1] = (0x0300) & 0xff;

    i2cWriteDevice(scd30, input_buffer, 2);
    i2cReadDevice(scd30, output_buffer, 18);

    co2 =         (output_buffer[0]  << 24) + (output_buffer[1]  << 16) + (output_buffer[3]  << 8) + (output_buffer[4]);
	temperature = (output_buffer[6]  << 24) + (output_buffer[7]  << 16) + (output_buffer[9]  << 8) + (output_buffer[10]);
	humidity =    (output_buffer[12] << 24) + (output_buffer[13] << 16) + (output_buffer[15] << 8) + (output_buffer[16]);

    // uint32_t to float: SCD3x_Interface_Description.pdf
    co2_float = * (float *) &co2;
    temperature_float = * (float *) &temperature;
    humidity_float = * (float *) &humidity;

    v_toarray1(retval, 3);
    v_setreal(v_elem(retval, 0), co2_float);
    v_setreal(v_elem(retval, 1), temperature_float);
    v_setreal(v_elem(retval, 2), humidity_float);

    return(1);
}

/* Set Temperature Offset
 * 0x5403 + TempertureOffset >= 0
 * Format: uint16 Temperature offset, unit [°C x 100], i.e.one tick corresponds to 0.01°C
 * The on-board RH/T sensor is influenced by thermal self-heating of
 * SCD30 and other electrical components. Design-in alters the thermal
 * properties of SCD30 such that temperature and humidity offsets may
 * occur when operating the sensor in end-customer devices.
 * Compensation of those effects is achievable by writing the
 * temperature offset found in continuous operation of the device into
 * the sensor. Temperature offset value is saved in non-volatile memory.
 * The last set value will be used for temperature offset compensation
 * after repowering.
 *
 * T_Offset,New = (T_SCD30 − T_Reference ) + T_Offset,Old
 * (PDF: Low power mode; https://sensirion.com/products/downloads/?filter_category=53cf3e00-c25a-41f5-a310-8e6837dcaa5f)
 * https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library/issues/27#issuecomment-971986826
 *
 * Since temp offset has to be send as °C*100, SmallBasic command will expect a float
 * of the temperature offset in °C and this value will be converted to °C*100
 */

int SCD30_BASIC_CMD_SetTemperatureOffset(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[5] = {0};

    uint16_t Temperature =  get_param_int(argc, params, 0, 0) * 100;

    if(Temperature >= 0 && Temperature <= 12500)
    {
        input_buffer[0] = 0x5403 >> 8;
        input_buffer[1] = 0x5403 & 0xff;
        input_buffer[2] = Temperature >> 8;
        input_buffer[3] = Temperature & 0xff;
        input_buffer[4] = gencrc(input_buffer);

        i2cWriteDevice(scd30, input_buffer, 5);

        return(1);
    }
    else
    {
        v_setstr(retval, "Invalid temperature offset (0..125)");
        return 0;
    }

}

/* (De-)Activate Automatic Self-Calibration (ASC)
 * 0x5306 + Actice/Deactive 0,1
 * Continuous automatic self-calibration can be (de-)activated
 * with the following command. When activated for the first time a
 * period of minimum 7 days is needed so that the algorithm can find
 * its initial parameter set for ASC. The sensor has to be exposed
 * to fresh air for at least 1 hour every day. Also during that period,
 * the sensor may not be disconnected from the power supply, otherwise
 * the procedure to find calibration parameters is aborted and has to
 * be restarted from the beginning. The successfully calculated
 * parameters are stored in non-volatile memory of the SCD30 having
 * the effect that after a restart the previously found parameters
 * for ASC are still present. Note that the most recently found
 * self-calibration parameters will be actively used for self-
 * calibration disregarding the status of this feature. Finding a new
 * parameter set by the here described method will always overwrite
 * the settings from external recalibration (see chapter 1.3.7) and
 * vice-versa. The feature is switched off by default. To work
 * properly SCD30 has to see fresh air on a regular basis.
 * Optimal working conditions are given when the sensor sees fresh
 * air for one hour every day so that ASC can constantly re-calibrate.
 * ASC only works in continuous measurement mode. ASC status is saved
 * in non-volatile memory. When the sensor is powered down while
 * ASC is activated SCD30 will continue with automatic self-calibration
 * after repowering without sending the command.
 */
int SCD30_BASIC_CMD_AutomaticSelfCalibration(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[5] = {0};

    int YesNo = get_param_int(argc, params, 0, 0);

    if(YesNo != 0) YesNo = 1;

    input_buffer[0] = (0x5306 >> 8) & 0xff;
	input_buffer[1] = (0x5306) & 0xff;
    input_buffer[2] = (YesNo >> 8) & 0xff;
	input_buffer[3] = (YesNo) & 0xff;
    input_buffer[4] = gencrc(input_buffer);

	i2cWriteDevice(scd30, input_buffer, 5);

    return(1);
}

/* Set Forced Recalibration value (FRC)
 * 0x5204 + CO2 Concentration in ppm
 * Forced recalibration (FRC) is used to compensate for sensor drifts
 * when a reference value of the CO2 concentration in close proximity
 * to the SCD30 is available. For best results the sensor has to be
 * run in a stable environment in continuous mode at a measurement
 * rate of 2s for at least two minutes before applying the calibration
 * command and sending the reference value. Setting a reference CO2
 * concentration by the here described method will always overwrite
 * the settings from ASC and vice-versa. The reference CO2 concentration
 * has to be within the range 400 ppm ≤ cref(CO2) ≤ 2000 ppm. FRC value
 * is saved in non-volatile memory, the last set FRC value will be
 * used for field-calibration after repowering.
 */
int SCD30_BASIC_CMD_CO2Recalibration(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[5] = {0};

    int CO2ReferenceValue    = get_param_int(argc, params, 0, 0);

    if(CO2ReferenceValue >= 400 && CO2ReferenceValue <= 2000)
    {
        input_buffer[0] = (0x5204 >> 8) & 0xff;
        input_buffer[1] = (0x5204) & 0xff;
        input_buffer[2] = (CO2ReferenceValue >> 8) & 0xff;
        input_buffer[3] = (CO2ReferenceValue) & 0xff;
        input_buffer[4] = gencrc(input_buffer);

        i2cWriteDevice(scd30, input_buffer, 5);

        return(1);
    }
    else
    {
        v_setstr(retval, "Invalid CO2 reference (400..2000)");
        return 0;
    }
}

/* Altitude Compensation
 * 0x5102 + Height over sea level in m (uint16)
 * Measurements of CO2 concentration based on the NDIR principle are
 * influenced by altitude. SCD30 offers to compensate deviations due
 * to altitude by using the following command. Setting altitude is
 * disregarded when an ambient pressure is given
 */
int SCD30_BASIC_CMD_AltitudeCompensation(int argc, slib_par_t *params, var_t *retval)
{
    char input_buffer[5] = {0};

    int Altitude    = get_param_int(argc, params, 0, 0);

    input_buffer[0] = (0x5403 >> 8) & 0xff;
	input_buffer[1] = (0x5403) & 0xff;
    input_buffer[2] = (Altitude >> 8) & 0xff;
	input_buffer[3] = (Altitude) & 0xff;
    input_buffer[4] = gencrc(input_buffer);

	i2cWriteDevice(scd30, input_buffer, 5);

    return(1);
}


int SCD30_BASIC_CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
	scd30 = i2cOpen(1, SCD30_ADDRESS, 0);

    if(scd30 < 0)
    {
        v_setstr(retval, "Cannot connect to sensor.");
        return (0);
    }

    return(1);
}

int SCD30_BASIC_CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
	i2cClose(scd30);

	return(1);
}
