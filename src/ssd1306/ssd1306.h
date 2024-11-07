 
//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2021

#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "var.h"
#include "param.h"

int CMD_Open(int argc, slib_par_t *params, var_t *retval);
void Close(void);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
int CMD_Display(int argc, slib_par_t *params, var_t *retval);
int CMD_ClearDisplay(int argc, slib_par_t *params, var_t *retval);
int CMD_Pset(int argc, slib_par_t *params, var_t *retval);
int CMD_Line(int argc, slib_par_t *params, var_t *retval);
int CMD_Rect(int argc, slib_par_t *params, var_t *retval);
int CMD_Circle(int argc, slib_par_t *params, var_t *retval);
int CMD_RoundRect(int argc, slib_par_t *params, var_t *retval);
int CMD_Triangle(int argc, slib_par_t *params, var_t *retval);
int CMD_Print(int argc, slib_par_t *params, var_t *retval);
int CMD_CopyToDisplay(int argc, slib_par_t *params, var_t *retval);
int CMD_SetTextSize(int argc, slib_par_t *params, var_t *retval);
int CMD_At(int argc, slib_par_t *params, var_t *retval);
int CMD_SetBrightness(int argc, slib_par_t *params, var_t *retval);
int CMD_InvertDisplay(int argc, slib_par_t *params, var_t *retval);
int CMD_CopyFromDisplay(int argc, slib_par_t *params, var_t *retval);
int CMD_DisplayOn(int argc, slib_par_t *params, var_t *retval);
int CMD_DisplayOff(int argc, slib_par_t *params, var_t *retval);
void CopyFramebuffer(uint8_t *FB_8bit, uint8_t *FB_1bit);

#endif /* !_SSD1306_H_ */
