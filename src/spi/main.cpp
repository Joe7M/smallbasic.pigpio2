//This file is part of the SmallBasicPIGPIO plugin to
//use I2Con a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "spi.h"

#include "var.h"
#include "module.h"
#include "param.h"

char *programSource = nullptr;

//Min. parameter , max. parameters, function name in Basic, function in c
FUNC_SIG lib_func[] =
{
  {0, 1, "Read", CMD_Read},
  {1, 2, "ReadReg", CMD_ReadReg},
  {1, 2, "ReadWrite", CMD_ReadWrite},
  {1, 1, "ReadWriteParallel", CMD_ReadWriteParallel},
  {0, 0, "GetSpeed", CMD_GetMaxSpeed},
  {0, 0, "GetLSBFirst", CMD_GetLSBFirst},
  {0, 0, "GetBitsPerWord", CMD_GetBitsPerWord},
  {0, 0, "GetMode", CMD_GetMode}
};

static FUNC_SIG lib_proc[] =
{
  {0, 2, "Open", CMD_Open},
  {0, 0, "Close", CMD_Close},
  {1, 2, "Write", CMD_Write},
  {2, 2, "WriteReg", CMD_WriteReg},
  {1, 1, "SetSpeed", CMD_SetMaxSpeed},
  {0, 1, "SetLSBFirst", CMD_SetLSBFirst},
  {1, 1, "SetMode", CMD_SetMode},
  {1, 1, "SetBitsPerWord", CMD_SetBitsPerWord},
  {1, 1, "SetDelay", CMD_SetDelay}
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
  Close();
  free(programSource);
}
