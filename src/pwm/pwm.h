//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#ifndef _PWM_H_
#define _PWM_H_

#include "var.h"
#include "param.h"


int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int CMD_SetPeriod(int argc, slib_par_t *params, var_t *retval);
int CMD_SetDutyCycle(int argc, slib_par_t *params, var_t *retval);
int CMD_Enable(int argc, slib_par_t *params, var_t *retval);

#endif /* !_PWM_H_ */
