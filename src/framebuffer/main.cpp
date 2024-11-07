//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas


#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "FB.hpp"

#include "var.h"
#include "module.h"
#include "param.h"

char *programSource = nullptr;


//Min Parameter , max Parameters, Function Name in Basic, Function in c 
FUNC_SIG lib_func[] =
{
  {1, 5, "GETARRAY",  CMD_CopyFromDisplay},
  {0, 0, "GETMOUSE",  CMD_GetMouse},
  {0, 1, "OPEN",      CMD_Init},
};


static FUNC_SIG lib_proc[] =
{
  {1, 1, "CLOSE",         CMD_Close},
  {1, 2, "CLEAR",         CMD_Clear},
  {3, 6, "CIRCLE",        CMD_Circle},
  {1, 3, "COLOR",         CMD_Color},
  {1, 5, "SETARRAY",      CMD_CopyToDisplay},
  {5, 6, "LINE",          CMD_Line},
  {5, 7, "RECT",          CMD_Rect},
  {6, 8, "ROUNDRECT",     CMD_RoundRect},
  {7, 9, "TRIANGLE",      CMD_Triangle},
  {2, 3, "PRINT",         CMD_Print},
  {3, 4, "SETPIXEL",      CMD_SetPixel},
  {2, 2, "SETTEXTSIZE",   CMD_SetTextSize},
  {1, 3, "AT",            CMD_At},
  {1, 1, "WAITFORVSYNC",  CMD_WaitForVSync},
  {1, 1, "SWAPBUFFER",    CMD_SwapBuffer}
};


SBLIB_API int sblib_proc_count()
{
  return (sizeof(lib_proc) / sizeof(lib_proc[0]));
}

int sblib_func_count()
{
  return (sizeof(lib_func) / sizeof(lib_func[0]));
}

SBLIB_API int sblib_proc_getname(int index, char *proc_name)
{
  int result;
  if (index < sblib_proc_count()) {
    strcpy(proc_name, lib_proc[index]._name);
    result = 1;
  } else {
    result = 0;
  }
  return result;
}



int sblib_func_getname(int index, char *proc_name)
{
  int result;
  if (index < sblib_func_count()) {
    strcpy(proc_name, lib_func[index]._name);
    result = 1;
  } else {
    result = 0;
  }
  return result;
}

SBLIB_API int sblib_proc_exec(int index, int argc, slib_par_t *params, var_t *retval) {
  int result;
  if (index >= 0 && index < sblib_proc_count()) {
    if (argc < lib_proc[index]._min || argc > lib_proc[index]._max) {
      if (lib_proc[index]._min == lib_proc[index]._max) {
        error(retval, lib_proc[index]._name, lib_proc[index]._min);
      } else {
        error(retval, lib_proc[index]._name, lib_proc[index]._min, lib_proc[index]._max);
      }
      result = 0;
    } else {
      result = lib_proc[index]._command(argc, params, retval);
    }
  } else {
    fprintf(stderr, "SmallBasicPIGPIO: PROC index error [%d]\n", index);
    result = 0;
  }
  return result;
}



SBLIB_API int sblib_func_exec(int index, int argc, slib_par_t *params, var_t *retval) {
  int result;
  if (index >= 0 && index < sblib_func_count()) {
    if (argc < lib_func[index]._min || argc > lib_func[index]._max) {
      if (lib_func[index]._min == lib_func[index]._max) {
        error(retval, lib_func[index]._name, lib_func[index]._min);
      } else {
        error(retval, lib_func[index]._name, lib_func[index]._min, lib_func[index]._max);
      }
      result = 0;
    } else {
      result = lib_func[index]._command(argc, params, retval);
    }
  } else {
    fprintf(stderr, "SmallBasicPIGPIO: FUNC index error [%d]\n", index);
    result = 0;
  }
  return result;
}

int sblib_init(const char *sourceFile)
{
  programSource = strdup(sourceFile);
  return 1;
}

void sblib_close(void)
{
  free(programSource);
}
