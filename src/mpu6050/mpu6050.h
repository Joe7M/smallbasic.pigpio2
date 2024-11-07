/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT license
 * Joerg Siebenmorgen, 2024
*/

#ifndef ADS1X15_H
#define ADS1X15_H

#include "var.h"
#include "param.h"

int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
void Close(void);
int CMD_GetAcceleration(int argc, slib_par_t *params, var_t *retval);
int CMD_GetGyroscope(int argc, slib_par_t *params, var_t *retval);
int CMD_GetTemperature(int argc, slib_par_t *params, var_t *retval);
int CMD_GyroscopeConfig(int argc, slib_par_t *params, var_t *retval);
int CMD_AccelerationConfig(int argc, slib_par_t *params, var_t *retval);

#endif

