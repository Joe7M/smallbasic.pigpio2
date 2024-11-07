//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#include "config.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "gpio.h"
#include <gpiod.h>

#define MAX_GPIO_LINE 53

struct gpiod_chip *gpiochip;
struct gpiod_line *gpiolines[MAX_GPIO_LINE];

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "gpiochip0");
  char *gpiochipname = new char[strlen(text) + 2];
  strcpy(gpiochipname, text);

  gpiochip = gpiod_chip_open_by_name(gpiochipname);
  if(gpiochip == NULL )
  {
    char buffer[30 + strlen(text) + 2];
    sprintf(buffer, "Error opening gpio chip %s", gpiochipname);
    v_setstr(retval, buffer);
    return(0);
  }
  return(1);
}

int Close(void)
{
  gpiod_chip_close(gpiochip);
  return(1);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  Close();
  return(1);
}

int CMD_SetInput(int argc, slib_par_t *params, var_t *retval)
{
  // Set the pin to be an input and set internal pullup resitor

  uint8_t Pin = get_param_int(argc, params, 0, 0);
  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if(gpiolines[Pin] != NULL) gpiod_line_release(gpiolines[Pin]);

  gpiolines[Pin] = gpiod_chip_get_line(gpiochip, Pin);
  if(gpiolines[Pin] == NULL)
  {
    v_setstr(retval, "Unable to get GPIO line. GPIO pin might be in use.");
    return(0);
  }

  if(gpiod_line_request_input_flags(gpiolines[Pin], "SmallBasicPIGPIO", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) == -1)
  {
    gpiod_line_release(gpiolines[Pin]);
    v_setstr(retval, "Unable to set input. GPIO pin might be in use.");
    return(0);
  }

  return(1);
}

int CMD_SetOutput(int argc, slib_par_t *params, var_t *retval)
{
  // Set the pin to be an output

  int Pin    = get_param_int(argc, params, 0, 0);
  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if(gpiolines[Pin] != NULL) gpiod_line_release(gpiolines[Pin]);

  gpiolines[Pin] = gpiod_chip_get_line(gpiochip, Pin);
  if(gpiolines[Pin] == NULL)
  {
    v_setstr(retval, "Unable to get GPIO line. GPIO pin might be in use.");
    return(0);
  }

  if(gpiod_line_request_output(gpiolines[Pin], "SmallBasicPIGPIO", 0) == -1)
  {
    gpiod_line_release(gpiolines[Pin]);
    v_setstr(retval, "Unable to set output. GPIO pin might be in use.");
    return(0);
  }

  return(1);
}

int CMD_Write(int argc, slib_par_t *params, var_t *retval)
{
  //Sets a Pin to high or low

  uint8_t Pin	= get_param_int(argc, params, 0, 0);
  bool HighLow	= get_param_int(argc, params, 1, 0);

  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  gpiod_line_set_value(gpiolines[Pin], HighLow);

  return(1);
}

int CMD_Read(int argc, slib_par_t *params, var_t *retval)
{
  //Read if Pin is high or low

  uint8_t Pin = get_param_int(argc, params, 0, 0);
  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  v_setint(retval, gpiod_line_get_value(gpiolines[Pin]));

  return(1);
}

int CMD_Trigger(int argc, slib_par_t *params, var_t *retval)
{
  // Emits a trigger pulse with a given pulse length in us
  // and given level. Max. pulse lenght is 65535us. Level is
  // optional. Default value is 1. Pulse lenght is optional.
  // Default value is 50us.

  uint8_t  Pin         = get_param_int(argc, params, 0, 0);
  uint8_t  Level       = get_param_int(argc, params, 1, 1);
  uint16_t PulseLength = get_param_int(argc, params, 2, 50);


  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  gpiod_line_set_value(gpiolines[Pin], Level);
  usleep(PulseLength);
  gpiod_line_set_value(gpiolines[Pin], !Level);

  return(1);
}

int CMD_WaitEvent(int argc, slib_par_t *params, var_t *retval)
{
  // Wait for a raising edge event at specified pin. Timeout_s in
  // an optional parameter to define a time out in s. Max 255 s.
  // Return values:
  //    -1: error
  //     0: time out
  //     1: rising edge detected

  struct timespec timeout;

  uint8_t Pin       = get_param_int(argc, params, 0, 0);
  uint8_t Timeout_s = get_param_int(argc, params, 1, 1);
  //uint8_t EventType = get_param_int(argc, params, 1, 0);  // TODO: Implement Raising or falling edge

  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if(gpiolines[Pin] != NULL) gpiod_line_release(gpiolines[Pin]);

  if(gpiod_line_request_rising_edge_events(gpiolines[Pin], "SmallBasicPIGPIO"))
  {
    v_setstr(retval, "Unable to get GPIO line. GPIO pin might be in use.");
    return(0);
  }

  timeout.tv_sec = Timeout_s;
  timeout.tv_nsec = 0;

  v_setint(retval, gpiod_line_event_wait(gpiolines[Pin], &timeout));

  return(1);
}

int CMD_ReleaseLine(int argc, slib_par_t *params, var_t *retval)
{
  // Release a line.

  uint8_t Pin = get_param_int(argc, params, 0, 0);

  if(Pin > MAX_GPIO_LINE)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if(gpiolines[Pin] != NULL) gpiod_line_release(gpiolines[Pin]);

  return(1);
}