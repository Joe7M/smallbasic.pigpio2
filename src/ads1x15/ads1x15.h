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
int CMD_Channel(int argc, slib_par_t *params, var_t *retval);
int CMD_SampleRate(int argc, slib_par_t *params, var_t *retval);
int CMD_Read(int argc, slib_par_t *params, var_t *retval);
int CMD_VoltageRange(int argc, slib_par_t *params, var_t *retval);
int CMD_ContinuousMode(int argc, slib_par_t *params, var_t *retval);
int CMD_SingleShotMode(int argc, slib_par_t *params, var_t *retval);

#endif

