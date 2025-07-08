// This file is part of the SmallBasicPIGPIO plugin to
// use gpio pins on a Raspberry pi with SmallBASIC.
// MIT licence
// Joerg Siebenmorgen, 2025
//
// SDL version	 :  sudo sbasicg -m/usr/local/lib/smallbasic/ -n pwm_1ch.bas
// Console version: sudo sbasic -m /usr/local/lib/smallbasic/ pwm_1ch.bas

// Based on: https://raspberrypi.stackexchange.com/questions/143643/how-can-i-use-dtoverlay-pwm
// More information: https://www.kernel.org/doc/html/v5.10/driver-api/pwm.html

#include "config.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "pwm.h"

#define ONE_KHZ_IN_NS 1000000

char *PWM_SysFS;

int WriteSysFS(const char filename[], uint32_t value)
{
  FILE* fp;
  char *file = new char[strlen(filename) + strlen(PWM_SysFS) + 2];
  strcpy(file, PWM_SysFS);
  strcat(file, filename);

  //printf("%s > %i\n", file, value);

  fp = fopen(file, "w");
  if (fp == NULL) return 1;
  fprintf(fp, "%i", value);

  fclose(fp);
  return 0;
}

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *PWM_Chip = get_param_str(argc, params, 0, "pwmchip0");
  PWM_SysFS = new char[strlen(PWM_Chip) + strlen("/sys/class/pwm/") + 2];
  strcpy(PWM_SysFS, "/sys/class/pwm/");
  strcat(PWM_SysFS, PWM_Chip);

  WriteSysFS("/export", 0);
  WriteSysFS("/export", 1);

  usleep(100000);

  // Setup default parameters

  // Total periode in ns
  if(WriteSysFS("/pwm0/period", ONE_KHZ_IN_NS))
  {
    v_setstr(retval, "Unable to write to pwm0/period.");
    return 0;
  }
  if(WriteSysFS("/pwm1/period", ONE_KHZ_IN_NS))
  {
    v_setstr(retval, "Unable to write to pwm1/period.");
    return 0;
  }
  // The active time of the PWM signal in ns
  if(WriteSysFS("/pwm0/duty_cycle", ONE_KHZ_IN_NS))
  {
    v_setstr(retval, "Unable to write to pwm0/duty_cycle.");
    return 0;
  }
  if(WriteSysFS("/pwm1/duty_cycle", ONE_KHZ_IN_NS))
  {
    v_setstr(retval, "Unable to write to pwm1/duty_cycle.");
    return 0;
  }
  // PWM off
  if(WriteSysFS("/pwm0/enable", 0))
  {
    v_setstr(retval, "Unable to write to pwm0/enable.");
    return 0;
  }
  if(WriteSysFS("/pwm1/enable", 0))
  {
    v_setstr(retval, "Unable to write to pwm1/enable.");
    return 0;
  }

  return(1);
}

int CMD_SetPeriod(int argc, slib_par_t *params, var_t *retval)
{

  uint32_t P = get_param_int(argc, params, 0, ONE_KHZ_IN_NS);
  uint32_t n = get_param_int(argc, params, 1, 0);

  if(n == 1)
  {
      if(WriteSysFS("/pwm1/period", P))
      {
        v_setstr(retval, "Unable to write to pwm1/period.");
        return 0;
      }
  }
  else
  {
    if(WriteSysFS("/pwm0/period", P))
    {
      v_setstr(retval, "Unable to write to pwm0/period.");
      return 0;
    }
  }
  return(1);
}

int CMD_SetDutyCycle(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t D = get_param_int(argc, params, 0, ONE_KHZ_IN_NS);
  uint32_t n = get_param_int(argc, params, 1, 0);

  if(n == 1)
  {
    if(WriteSysFS("/pwm1/duty_cycle", D))
    {
      v_setstr(retval, "Unable to write to pwm1/duty_cycle.");
      return 0;
    }
  }
  else
  {
    if(WriteSysFS("/pwm0/duty_cycle", D))
    {
      v_setstr(retval, "Unable to write to pwm0/duty_cycle.");
      return 0;
    }
  }
  return(1);
}

int CMD_Enable(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t E = get_param_int(argc, params, 0, 1);
  uint32_t n = get_param_int(argc, params, 1, 0);

  if(n == 1)
  {
    if(WriteSysFS("/pwm1/enable", E))
    {
      v_setstr(retval, "Unable to write to pwm1/enable.");
      return 0;
    }
  }
  else
  {
    if(WriteSysFS("/pwm0/enable", E))
    {
      v_setstr(retval, "Unable to write to pwm0/enable.");
      return 0;
    }
  }
  return(1);
}
