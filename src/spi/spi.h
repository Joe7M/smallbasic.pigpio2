//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#ifndef SRC_SPI_H_
#define SRC_SPI_H_

#include "var.h"
#include "param.h"

int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int Close(void);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
int CMD_Read(int argc, slib_par_t *params, var_t *retval);
int CMD_ReadReg(int argc, slib_par_t *params, var_t *retval);
int CMD_ReadWrite(int argc, slib_par_t *params, var_t *retval);
int CMD_Write(int argc, slib_par_t *params, var_t *retval);
int CMD_GetMode(int argc, slib_par_t *params, var_t *retval);
int CMD_SetMode(int argc, slib_par_t *params, var_t *retval);
int CMD_GetMaxSpeed(int argc, slib_par_t *params, var_t *retval);
int CMD_SetMaxSpeed(int argc, slib_par_t *params, var_t *retval);
int CMD_SetBitsPerWord(int argc, slib_par_t *params, var_t *retval);
int CMD_GetBitsPerWord(int argc, slib_par_t *params, var_t *retval);
int CMD_SetDelay(int argc, slib_par_t *params, var_t *retval);
int CMD_SetLSBFirst(int argc, slib_par_t *params, var_t *retval);
int CMD_GetLSBFirst(int argc, slib_par_t *params, var_t *retval);

#endif /* !SRC_SPI_H_ */