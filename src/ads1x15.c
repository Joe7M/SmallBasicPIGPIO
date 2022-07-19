/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT license
 * Joerg Siebenmorgen, 2022
*/

/*
 * ADS1x15 source code is based on
 * lgl_ads1x15.c
 * 2021-01-16
 * Public Domain
 * http://abyz.me.uk/lg/lgpio.html
 * http://abyz.me.uk/lg/rgpio.html
*/

#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "ads1x15.h"


/*
   ada1015 4 ch 12-bit ADC 3300 sps
   ads1115 4 ch 16-bit ADC 860 sps

   F E D C   B A 9 8   7 6 5 4   3 2 1 0
   Z C C C   V V V S   R R R W   P L Q Q

   Z    1=start conversion
   CCC  0=A0/A1(*) 1=A0/A3 2=A1/A3 3=A2/A3 4=A0 5=A1 6=A2 7=A3
   VVV  0=6.144 1=4.096 2=2.048(*) 3=1.024 4=0.512 5=0.256 6=0.256 7=0.256
   S    0=continuous 1=single shot(*)
   RRR  0=8   1=16  2=32  3=64  4=128  5=250  6=475  7=860 sps ADS1115
   RRR  0=128 1=250 2=490 3=920 4=1600(*) 5=2400 6=3300 7=3300 sps ADS1015
   W    0=traditional(*) 1=window
   P    ALERT/RDY pin 0=active low(*)  1=active high
   L    comparator 0=non-latching(*) 1=latching
   QQ   queue 0=after 1 1=after 2, 2=after 4, 3=disable(*)
*/

#define ADS1X15_A0_1 0
#define ADS1X15_A0_3 1
#define ADS1X15_A1_3 2
#define ADS1X15_A2_3 3
#define ADS1X15_A0   4
#define ADS1X15_A1   5
#define ADS1X15_A2   6
#define ADS1X15_A3   7

#define ADS1X15_ALERT_NEVER       0
#define ADS1X15_ALERT_READY       1
#define ADS1X15_ALERT_TRADITIONAL 2
#define ADS1X15_ALERT_WINDOW      3

#define CONVERSION_REG 0
#define CONFIG_REG 1
#define COMPARE_LOW_REG 2
#define COMPARE_HIGH_REG 3

typedef struct ads1x15_s
{
   int sbc;       // sbc connection (unused)
   int bus;       // I2C bus
   int device;    // I2C device
   int flags;     // I2C flags
   int i2ch;      // I2C handle
   int alert_rdy; // mode of ALERT_RDY pin
   int configH;   // config high byte
   int configL;   // config low byte
   int channel;   // channel setting
   int gain;      // gain setting
   int *SPS;      // array of legal samples per seconds
   float voltage_range; // voltage range (set by gain setting)
   float vhigh;   // alert high voltage
   float vlow;    // alert low voltage
   int single_shot; // single shot setting
   int sps;       // samples per second setting
   int comparator_mode;
   int comparator_polarity;
   int comparator_latch;
   int comparator_queue;
   int compare_high; // set from vhigh
   int compare_low;  // set from vlow
   int set_queue;    // set from comparator queue
} ads1x15_t, *ads1x15_p;

//typedef struct ads1x15_s ads1x15_t, *ads1x15_p;

ads1x15_p adc=NULL;

float _GAIN[] = {6.144, 4.096, 2.048, 1.024, 0.512, 0.256, 0.256, 0.256};

const char *_CHAN[] = {"A0-A1", "A0-A3", "A1-A3", "A2-A3", "A0", "A1", "A2", "A3"};

int _SPS_1015[] = {128, 250, 490, 920, 1600, 2400, 3300, 3300};
int _SPS_1115[] = {  8,  16,  32,  64,  128,  250,  475,  860};

ads1x15_p ADS1015_open(int sbc, int bus, int device, int flags);
ads1x15_p ADS1115_open(int sbc, int bus, int device, int flags);
ads1x15_p ADS1X15_close(ads1x15_p s);
int ADS1X15_set_channel(ads1x15_p s, int channel);
int ADS1X15_get_channel(ads1x15_p s);
const char * ADS1X15_get_channel_name(ads1x15_p s);
float ADS1X15_set_voltage_range(ads1x15_p s, float vrange);
float ADS1X15_get_voltage_range(ads1x15_p s);
int ADS1X15_set_sample_rate(ads1x15_p s, int rate);
int ADS1X15_get_sample_rate(ads1x15_p s);
int ADS1X15_set_continuous_mode(ads1x15_p s);
int ADS1X15_set_single_shot_mode(ads1x15_p s);
int ADS1X15_get_conversion_mode(ads1x15_p s);
int ADS1X15_read(ads1x15_p s);
float ADS1X15_read_voltage(ads1x15_p s);
int ADS1X15_alert_never(ads1x15_p s);
int ADS1X15_alert_when_high_clear_when_low(ads1x15_p s, float vhigh, float vlow);
int ADS1X15_alert_when_high_or_low(ads1x15_p s, float vhigh, float vlow);
int ADS1X15_alert_when_ready(ads1x15_p s);
int ADS1X15_set_comparator_latch(ads1x15_p s, int value);
int ADS1X15_get_comparator_latch(ads1x15_p s);
int ADS1X15_set_comparator_polarity(ads1x15_p s, int level);
int ADS1X15_get_comparator_polarity(ads1x15_p s);
int ADS1X15_set_comparator_queue(ads1x15_p s, int queue);
int ADS1X15_get_comparator_queue(ads1x15_p s);






int _read_config(ads1x15_p s)
{
   char buf[8];

   buf[0] = CONFIG_REG;

   i2cWriteDevice(s->i2ch, buf, 1);  // set config register

   i2cReadDevice(s->i2ch, buf, 2);

   s->configH = buf[0];
   s->configL = buf[1];

   buf[0] = COMPARE_LOW_REG;

   i2cWriteDevice(s->i2ch, buf, 1); // set low compare register

   i2cReadDevice(s->i2ch, buf, 2);

   s->compare_low = (buf[0] << 8) | buf[1];

   buf[0] = COMPARE_HIGH_REG;

   i2cWriteDevice(s->i2ch, buf, 1); // set high compare register

   i2cReadDevice(s->i2ch, buf, 2);

   s->compare_high = (buf[0] << 8) | buf[1];

   buf[0] = CONVERSION_REG;

   i2cWriteDevice(s->i2ch, buf, 1); // set conversion register

   s->channel = (s->configH >> 4) & 7;
   s->gain = (s->configH >> 1) & 7;
   s->voltage_range = _GAIN[s->gain];
   s->single_shot = s->configH & 1;
   s->sps = (s->configL >> 5) & 7;
   s->comparator_mode = (s->configL >> 4) & 1;
   s->comparator_polarity = (s->configL >> 3) & 1;
   s->comparator_latch = (s->configL >> 2) & 1;
   s->comparator_queue = s->configL & 3;

   if (s->comparator_queue != 3)
      s->set_queue = s->comparator_queue;
   else
      s->set_queue = 0;

   return 0;
}

int _write_config(ads1x15_p s)
{
   char buf[8];

   buf[0] = CONFIG_REG;
   buf[1] = s->configH;
   buf[2] = s->configL;

   i2cWriteDevice(s->i2ch, buf, 3);

   buf[0] = CONVERSION_REG;

   i2cWriteDevice(s->i2ch, buf, 1);

   return 0;
}

int _write_comparator_thresholds(ads1x15_p s, int high, int low)
{
   char buf[8];

   if (high > 32767) high = 32767;
   else if (high < -32768) high = -32768;

   if (low > 32767) low = 32767;
   else if (low < -32768) low = -32768;

   s->compare_high = high;
   s->compare_low = low;

   buf[0] = COMPARE_LOW_REG;
   buf[1] = (low >> 8) & 0xff;
   buf[2] = low & 0xff;

   i2cWriteDevice(s->i2ch, buf, 3);

   buf[0] = COMPARE_HIGH_REG;
   buf[1] = (high >> 8) & 0xff;
   buf[2] = high & 0xff;

   i2cWriteDevice(s->i2ch, buf, 3);

   buf[0] = CONVERSION_REG;

   i2cWriteDevice(s->i2ch, buf, 1);

   return 0;
}

int _update_comparators(ads1x15_p s)
{
   int h, l;

   if (s->alert_rdy >= ADS1X15_ALERT_TRADITIONAL)
   {
      h = s->vhigh * 32768.0 / s->voltage_range;
      l = s->vlow * 32768.0 / s->voltage_range;

      return _write_comparator_thresholds(s, h, l);
   }

   return 0;
}

int _update_config(ads1x15_p s)
{
   int H, L;

   H = s->configH;
   L = s->configL;

   s->configH = ((1 << 7) | (s->channel << 4) |
      (s->gain << 1) | s->single_shot);

   s->configL = ((s->sps << 5) | (s->comparator_mode << 4) |
      (s->comparator_polarity << 3) | (s->comparator_latch << 2) |
      s->comparator_queue);

   if ((H != s->configH) || (L != s->configL)) _write_config(s);

   return 0;
}

int ADS1X15_set_comparator_polarity(ads1x15_p s, int level)
/*
Set the level of the ALERT/RDY pin when active.

1 for active high, 0 for active low.
*/
{
   if (level) s->comparator_polarity = 1;
   else s->comparator_polarity = 0;

   return _update_config(s);
}

int ADS1X15_get_comparator_polarity(ads1x15_p s)
   { return s->comparator_polarity; }

int ADS1X15_set_comparator_latch(ads1x15_p s, int value)
/*
Sets whether the ALERT/RDY pin stays active when asserted
or whether it is cleared automatically if the alert condition
is no longer present.

True or non-zero sets latch on, False or 0 sets latch off.
*/
{
      if (value) s->comparator_latch = 1;
      else       s->comparator_latch = 0;

      return _update_config(s);
}

int ADS1X15_get_comparator_latch(ads1x15_p s)
{ 
	return s->comparator_latch;
}


int ADS1X15_set_comparator_queue(ads1x15_p s, int queue)
/*
Sets the number of consecutive alert readings required
before asserting the ALERT/RDY pin.

0 for 1 reading
1 for 2 readings
2 for 4 readings
*/
{
   if ((queue >= 0) && (queue <= 2))
   {
      s->set_queue = queue;
      return _update_config(s);
   }

   return -1;
}

int ADS1X15_get_comparator_queue(ads1x15_p s)
{ 
	return s->set_queue;
}


int ADS1X15_set_continuous_mode(ads1x15_p s)
{
      s->single_shot = 0;
      return _update_config(s);
}

int ADS1X15_set_single_shot_mode(ads1x15_p s)
{
   s->single_shot = 1;
   return _update_config(s);
}

int ADS1X15_get_conversion_mode(ads1x15_p s)
{ 
	return s->single_shot;
}

int ADS1X15_set_sample_rate(ads1x15_p s, int rate)
/*
Set the sample rate.

Any value less than the minimum will set the minimum rate.  Any
value greater than the maximum will set the maximum rate.

The ADS1015 supports 128, 250, 490, 920, 1600, 2400, and 3300
samples per second.

The ADS1115 supports 8, 16, 32, 64, 128, 250, 475, and 860 samples
per second.

The first sample rate greater than or equal to the specified
value will be set,

The set samples per second will be returned.
*/
{
   int val, i;

   val = 7;

   for (i=0; i<8; i++)
   {
      if (rate <= s->SPS[i])
      {
         val = i;
         break;
      }
   }

   s->sps = val;
   _update_config(s);

   return s->SPS[val];
}

int ADS1X15_get_sample_rate(ads1x15_p s)
{
	return s->SPS[s->sps];
}

float ADS1X15_set_voltage_range(ads1x15_p s, float vrange)
/*
Set the voltage range.

Any value less than the minimum will set the minimum rate.  Any
value greater than the maximum will set the maximum rate.

The ADS1015/ADS1115 support voltage ranges 256 mV, 512 mV, 1.024 V,
2.048 V, 4.096 V, and 6.144 V.

The first range greater than or equal to the specified value will
be set,

The set voltage range will be returned.
*/
{
   int val, i;

   val = 7;

   for (i=0; i<8; i++)
   {
      if (vrange > _GAIN[i])
      {
         val = i;
         break;
      }
   }

   if (val > 0) val = val - 1;

   s->gain = val;

   s->voltage_range = _GAIN[val];

   _update_comparators(s);

   _update_config(s);

   return s->voltage_range;
}

float ADS1X15_get_voltage_range(ads1x15_p s)
{ 
	return _GAIN[s->gain];
}


int ADS1X15_set_channel(ads1x15_p s, int channel)
/*
Set the channel to be used for conversions.

May be one of the following constants:

ADS1X15_A0 - single-ended A0
ADS1X15_A1 - single-ended A1
ADS1X15_A2 - single-ended A2
ADS1X15_A3 - single-ended A3

ADS1X15_A0_1 - differential A0 - A1
ADS1X15_A0_3 - differential A0 - A3
ADS1X15_A1_3 - differential A1 - A3
ADS1X15_A2_3 - differential A2 - A3

Returns the channel set.
*/
{
   if (channel < 0) channel = 0;
   else if (channel > 7) channel = 7;

   s->channel = channel;

   _update_config(s);

   return channel;
}

int   ADS1X15_get_channel(ads1x15_p s)
{
	return s->channel;
}

const char*ADS1X15_get_channel_name(ads1x15_p s)
{
	return _CHAN[s->channel];
}

int ADS1X15_alert_when_high_clear_when_low(ads1x15_p s, float vhigh, float vlow)
/*
Set the ALERT/RDY pin to be used as an alert signal.

The ALERT/RDY pin will be asserted when the voltage goes
above high.  It will continue to be asserted until the
voltage drops beneath low.

This sets continuous conversion mode.

To be useful your script will have to monitor the
ALERT/RDY pin and react when it changes level (both
edges is probably best).
*/
{
   s->vhigh = vhigh;
   s->vlow = vlow;

   s->comparator_mode = 0; // traditional
   s->comparator_queue = s->set_queue;
   s->alert_rdy = ADS1X15_ALERT_TRADITIONAL;
   _update_comparators(s);
   s->single_shot = 0; // must be in continuous mode
   return _update_config(s);
}

int ADS1X15_alert_when_high_or_low(ads1x15_p s, float vhigh, float vlow)
/*
Set the ALERT/RDY pin to be used as an alert signal.

The ALERT/RDY pin will be asserted when the voltage goes
above high or drops beneath low.  It will continue to be
asserted until the voltage is between low to high.

This sets continuous conversion mode.

To be useful your script will have to monitor the
ALERT/RDY pin and react when it changes level (both
edges is probably best).
*/
{
   s->vhigh = vhigh;
   s->vlow = vlow;

   s->comparator_mode = 1; // window
   s->comparator_queue = s->set_queue;
   s->alert_rdy = ADS1X15_ALERT_WINDOW;
   _update_comparators(s);
   s->single_shot = 0; // must be in continuous mode
   return _update_config(s);
}

int ADS1X15_alert_when_ready(ads1x15_p s)
/*
Set the ALERT/RDY pin to be used as a conversion
complete signal.

This sets continuous conversion mode.

To be useful your script will have to monitor the
ALERT/RDY pin and react when it is asserted (use
falling edge if comparator polarity is the default 0,
rising edge if comparator polarity is 1).
*/
{
   _write_comparator_thresholds(s, -1, 1); // conversion alerts
   s->comparator_queue = s->set_queue;
   s->alert_rdy = ADS1X15_ALERT_READY;
   s->single_shot = 0; // must be in continuous mode
   return _update_config(s);
}

int ADS1X15_alert_never(ads1x15_p s)

/* Set the ALERT/RDY pin to unused. */
{
   s->comparator_queue = 3;
   s->compare_high = 0;
   s->compare_low = 0;
   s->alert_rdy = ADS1X15_ALERT_NEVER;
   return _update_config(s);
}

int ADS1X15_get_alert_data(ads1x15_p s, int *high, int *low)
{
   *high = s->compare_high;
   *low = s->compare_low;
   return s->alert_rdy;
}

int ADS1X15_read_config_data(ads1x15_p s, int *high, int *low)
{
   _read_config(s);

   *high = s->compare_high;
   *low = s->compare_low;
   return (s->configH << 8) | s->configL;
}

int ADS1X15_read(ads1x15_p s)
/*
Returns the last conversion value.  For the ADS1115 this is a
16-bit quantity,  For the ADS1015 this is a 12-bit quantity.
The returned ADS1015 value is multiplied by 16 so it has the
same range as the ADS1115.
*/
{
   char buf[8];

   if (s->single_shot) _write_config(s);
  
   i2cReadDevice(s->i2ch, buf, 2);

   return (buf[0]<<8) + buf[1];
}

float ADS1X15_read_voltage(ads1x15_p s)
/* Returns the last conversion value as a voltage. */
{
   return ADS1X15_read(s) * s->voltage_range / 32768.0;
}


ads1x15_p ADS1X15_open(int sbc, int bus, int device, int flags)
{
   ads1x15_p s;

   s = (ads1x15_p)calloc(1, sizeof(ads1x15_t));

   if (s == NULL) return NULL;

   s->sbc = sbc;         // sbc connection
   s->bus = bus;         // I2C bus
   s->device = device;   // I2C device (address)
   s->flags = flags;     // I2C flags

   s->SPS = _SPS_1015;   // default

   s->i2ch = i2cOpen(bus, device, flags);

   if (s->i2ch < 0)
   {
      free(s);
      return NULL;
   }

   _read_config(s);

   ADS1X15_alert_never(s); // switch off ALERT/RDY pin.

   return s;
}

ads1x15_p ADS1015_open(int sbc, int bus, int device, int flags)
{
   ads1x15_p s;

   s = ADS1X15_open(sbc, bus, device, flags);

   if (s) s->SPS = _SPS_1015;

   return s;
}

ads1x15_p ADS1115_open(int sbc, int bus, int device, int flags)
{
   ads1x15_p s;

   s = ADS1X15_open(sbc, bus, device, flags);

   if (s) s->SPS = _SPS_1115;

   return s;
}

ads1x15_p ADS1X15_close(ads1x15_p s)
{
   if (s != NULL)
   {
      i2cClose(s->i2ch);
      free(s);
      s = NULL;
   }
   return s;
}


// ADS1X15_A0: 4 - single-ended A0
// ADS1X15_A1: 5 - single-ended A1
// ADS1X15_A2: 6 - single-ended A2
// ADS1X15_A3: 7 - single-ended A3
// ADS1X15_A0_1: 0 - differential A0 - A1
// ADS1X15_A0_3: 1 - differential A0 - A3
// ADS1X15_A1_3: 2 - differential A1 - A3
// ADS1X15_A2_3: 3 - differential A2 - A3
int BASIC_CMD_ADS1X15_SetChannel(int argc, slib_par_t *params, var_t *retval)
{
    int Channel = get_param_int(argc, params, 0, 4);

    ADS1X15_set_channel(adc, Channel);

    return 1;

}

// Address ADS11x5:
//   ADDR-Pin connected to
//    GND -> 0x48
//    VDD -> 0x49
//    SDA -> 0x50
//    SCL -> 0x51
int BASIC_CMD_ADS1X15_Open(int argc, slib_par_t *params, var_t *retval)
{
    int Address = get_param_int(argc, params, 0, 0x48);

    adc = ADS1015_open(0, 1, Address, 0);

    if(adc == NULL)
    {
        v_setstr(retval, "Cannot connect to sensor.");
        return (0);
    }

    return(1);
}


int BASIC_CMD_ADS1X15_Close(int argc, slib_par_t *params, var_t *retval)
{
	ADS1X15_close(adc);

	return(1);
}


/*
Set the voltage range. Any value less than the minimum will set the minimum rate.
Any value greater than the maximum will set the maximum rate. The ADS1015/ADS1115
support voltage ranges 256 mV, 512 mV, 1.024 V, 2.048 V, 4.096 V, and 6.144 V.
The first range greater than or equal to the specified value will be set.
*/
int BASIC_CMD_ADS1X15_SetVoltageRange(int argc, slib_par_t *params, var_t *retval)
{
    int VoltageRange = get_param_int(argc, params, 0, 6.144);

    ADS1X15_set_voltage_range(adc, VoltageRange);

    return 1;

}


/*
Set the sample rate. Any value less than the minimum will set the minimum rate.
Any value greater than the maximum will set the maximum rate. The ADS1015
supports 128, 250, 490, 920, 1600, 2400, and 3300 samples per second.
The ADS1115 supports 8, 16, 32, 64, 128, 250, 475, and 860 samples per second.
The first sample rate greater than or equal to the specified value will be set,
*/
int BASIC_CMD_ADS1X15_SetSampleRate(int argc, slib_par_t *params, var_t *retval)
{
    int SampelRate = get_param_int(argc, params, 0, 0);

    ADS1X15_set_sample_rate(adc, SampelRate);

    return 1;

}

int BASIC_CMD_ADS1X15_ReadVoltage(int argc, slib_par_t *params, var_t *retval)
{
    float Voltage;

    Voltage = ADS1X15_read_voltage(adc);

    v_setreal(retval, Voltage);

    return 1;

}

int BASIC_CMD_ADS1X15_SetContinuousMode(int argc, slib_par_t *params, var_t *retval)
{
   ADS1X15_set_continuous_mode(adc);

   return 1;
}

int BASIC_CMD_ADS1X15_SetSingleShotMode(int argc, slib_par_t *params, var_t *retval)
{
   ADS1X15_set_single_shot_mode(adc);

   return 1;
}


