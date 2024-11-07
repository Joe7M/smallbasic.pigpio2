//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "var.h"
#include "param.h"

int CMD_Open(int argc, slib_par_t *params, var_t *retval);
int Close(void);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
int CMD_Read(int argc, slib_par_t *params, var_t *retval);
int CMD_Write(int argc, slib_par_t *params, var_t *retval);
int CMD_Read_Reg(int argc, slib_par_t *params, var_t *retval);
int CMD_Write_Reg(int argc, slib_par_t *params, var_t *retval);
int CMD_Smbus_ReadReg_Byte(int argc, slib_par_t *params, var_t *retval);
int CMD_Smbus_ReadReg_Word(int argc, slib_par_t *params, var_t *retval);
int CMD_Smbus_Read_Byte(int argc, slib_par_t *params, var_t *retval);
int CMD_Smbus_Write_Byte(int argc, slib_par_t *params, var_t *retval);
int CMD_Smbus_Write_Word(int argc, slib_par_t *params, var_t *retval);

#endif /* !SRC_I2C_H_ */ 