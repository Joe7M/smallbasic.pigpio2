//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas


#ifndef _FB_H_
#define _FB_H_

#include "var.h"
#include "param.h"
#include <linux/fb.h>
#include "FBdraw.hpp"


class LinuxFrameBufferDraw : public FBDraw
{
  public:
    void SetDoubleBuffering(uint8_t TrueFalse) {UseDoubleBuffer = TrueFalse;};
    uint8_t GetDoubleBuffering(void) {return UseDoubleBuffer;};
    int32_t GetFrameBufferFileID(void) {return fbfd;};
    void SetFrameBufferFileID(int32_t f) {fbfd = f;};
    void SwapBuffers(void);
    void SetVinfo(struct fb_var_screeninfo vi) {vinfo = vi;};

  protected:
    int32_t fbfd;
    uint8_t UseDoubleBuffer = false;
    uint8_t CurrentDoubleBufferPage = 0;
    struct fb_var_screeninfo vinfo;
};


int CMD_Clear(int argc, slib_par_t *params, var_t *retval);
int CMD_Circle(int argc, slib_par_t *params, var_t *retval);
int CMD_Close(int argc, slib_par_t *params, var_t *retval);
int CMD_Color(int argc, slib_par_t *params, var_t *retval);
int CMD_CopyFromDisplay(int argc, slib_par_t *params, var_t *retval);
int CMD_CopyToDisplay(int argc, slib_par_t *params, var_t *retval);
int CMD_Init(int argc, slib_par_t *params, var_t *retval);
int CMD_Line(int argc, slib_par_t *params, var_t *retval);
int CMD_Rect(int argc, slib_par_t *params, var_t *retval);
int CMD_RoundRect(int argc, slib_par_t *params, var_t *retval);
int CMD_SetPixel(int argc, slib_par_t *params, var_t *retval);
int CMD_Triangle(int argc, slib_par_t *params, var_t *retval);
int CMD_Print(int argc, slib_par_t *params, var_t *retval);
int CMD_SetTextSize(int argc, slib_par_t *params, var_t *retval);
int CMD_At(int argc, slib_par_t *params, var_t *retval);
int CMD_WaitForVSync(int argc, slib_par_t *params, var_t *retval);
int CMD_SwapBuffer(int argc, slib_par_t *params, var_t *retval);
int CMD_GetMouse(int argc, slib_par_t *params, var_t *retval);
int CMD_GetInfo(int argc, slib_par_t *params, var_t *retval);

#endif /* !_FB_H_ */ 
