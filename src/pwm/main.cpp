// This file is part of the SmallBasicPIGPIO plugin to
// use gpio pins on a Raspberry pi with SmallBASIC.
// MIT licence
// Joerg Siebenmorgen, 2025
//
// SDL version	  : sudo sbasicg -m/usr/local/lib/smallbasic/ -n pwm_1ch.bas
// Console version: sudo sbasic -m /usr/local/lib/smallbasic/ pwm_1ch.bas

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "pwm.h"
#include "var.h"
#include "module.h"
#include "param.h"

char *programSource = nullptr;

static FUNC_SIG lib_proc[] =
{
  {0, 1, "Open", CMD_Open},
  {1, 2, "Period", CMD_SetPeriod},
  {1, 2, "DutyCycle", CMD_SetDutyCycle},
  {0, 2, "Enable", CMD_Enable}
};

SBLIB_API int sblib_proc_count()
{
  return (sizeof(lib_proc) / sizeof(lib_proc[0]));
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

int sblib_init(const char *sourceFile)
{
  programSource = strdup(sourceFile);
  return 1;
}

void sblib_close(void)
{
  free(programSource);
}
