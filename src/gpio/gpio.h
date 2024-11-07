//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas


#ifndef _GPIO_H_
#define _GPIO_H_

#include "var.h"
#include "param.h"

int Close(void);
int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
int CMD_SetInput(int argc, slib_par_t *params, var_t *retval);
int CMD_SetOutput(int argc, slib_par_t *params, var_t *retval);
int CMD_Write(int argc, slib_par_t *params, var_t *retval);
int CMD_Read(int argc, slib_par_t *params, var_t *retval);
int CMD_Pwm(int argc, slib_par_t *params, var_t *retval);
int CMD_Trigger(int argc, slib_par_t *params, var_t *retval);
int CMD_WaitEvent(int argc, slib_par_t *params, var_t *retval);
int CMD_ReleaseLine(int argc, slib_par_t *params, var_t *retval);

#endif /* !_GPIO_H_ */ 