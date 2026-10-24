/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT licence, Joerg Siebenmorgen, 2026
*/

#ifndef _HD44780_H_
#define _HD44780_H_

#include "var.h"
#include "param.h"

int Close(void);
int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int CMD_Print(int argc, slib_par_t *params, var_t *retval);
int CMD_Cls(int argc, slib_par_t *params, var_t *retval);
int CMD_On(int argc, slib_par_t *params, var_t *retval);
int CMD_Off(int argc, slib_par_t *params, var_t *retval);
int CMD_Locate(int argc, slib_par_t *params, var_t *retval);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);

#endif /* !_HD44780_H_ */
