/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT license
 * Joerg Siebenmorgen, 2024
*/

#ifndef BMP180_H
#define BMP180_H

#include "var.h"
#include "param.h"

int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
void Close(void);
int CMD_StartMeasurement(int argc, slib_par_t *params, var_t *retval);

#endif

