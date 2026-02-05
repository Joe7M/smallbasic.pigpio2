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
#include <time.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "gpio.h"
#include <gpiod.h>

#define MAX_GPIO_LINES 53
#define OUTPUT 1
#define INPUT 2
#define TRIGGER 3

uint8_t PinSettings[MAX_GPIO_LINES] = {0};
unsigned int TriggerEdgeDirection = 0;
unsigned int TriggerBias = 0;
unsigned int InputBias = 0;

char *gpiochipname;
struct gpiod_line_request *RequestOutput = NULL;
struct gpiod_line_request *RequestInput = NULL;
struct gpiod_line_request *RequestTrigger = NULL;

uint64_t millis()
{
  // copied from SmallBASIC stytem.c
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  if (0 == clock_gettime(CLOCK_MONOTONIC, &t)) {
    return (uint64_t) (1000L * t.tv_sec + (t.tv_nsec / 1e6));
  } else {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (uint64_t) (1000L * now.tv_sec + (now.tv_usec / 1000.0));
  }
}

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "/dev/gpiochip0");
  gpiochipname = new char[strlen(text) + 2];
  strcpy(gpiochipname, text);
  return(1);
}

int Close(void)
{
  if (RequestInput) gpiod_line_request_release(RequestInput);
  if (RequestOutput) gpiod_line_request_release(RequestOutput);
  if (RequestTrigger) gpiod_line_request_release(RequestTrigger);
  for(int ii = 0; ii <MAX_GPIO_LINES; ii++)
  {
    PinSettings[ii] = 0;
  }
  return(1);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  Close();
  return(1);
}

int RequestLine(int Pin, int Type, int EdgeDirection, int Bias)
{
  if(Pin > MAX_GPIO_LINES) return(1);

  if (Pin >= 0)
  {
    if (PinSettings[Pin] == Type) return(0);
    if (PinSettings[Pin] == INPUT || PinSettings[Pin] == OUTPUT || PinSettings[Pin] == TRIGGER) return(2);
    PinSettings[Pin] = Type;
  }

  uint32_t count = 0;
  uint32_t gpioPins[MAX_GPIO_LINES];
  for (uint32_t ii = 0; ii < MAX_GPIO_LINES; ii++)
  {
    if (PinSettings[ii] == Type)
    {
      gpioPins[count] = ii;
      count++;
    }
  }

  struct gpiod_request_config *req_cfg = NULL;
  struct gpiod_line_settings *settings;
  struct gpiod_line_config *line_cfg;
  struct gpiod_chip *chip;
  int ret, result = 0;

  chip = gpiod_chip_open(gpiochipname);
  if (!chip) return(3);

  settings = gpiod_line_settings_new();
  if (!settings)
  {
    result = 4;
    goto close_chip;
  }

  switch (Type)
  {
    case TRIGGER:
      if (EdgeDirection == 3)
      {
        gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_BOTH);
      }
      else if (EdgeDirection == 1)
      {
        gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_FALLING);
      }
      else
      {
        gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_RISING);
      }
    case INPUT:
      gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
      if (Bias == 2)
      {
        gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_DISABLED);
      }
      else if (Bias == 1)
      {
        gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_DOWN);
      }
      else
      {
        gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
      }
      break;
    case OUTPUT:
      gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
      gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);
      break;
  }
    
  line_cfg = gpiod_line_config_new();
  if (!line_cfg)
  {
    result = 5;
    goto free_settings;
  }

  ret = gpiod_line_config_add_line_settings(line_cfg, gpioPins, count, settings);
  if (ret)
  {
    result = 6;
    goto free_line_config;
  }

  req_cfg = gpiod_request_config_new();
  if (!req_cfg)
  {
    result = 7;
    goto free_line_config;
  }
  gpiod_request_config_set_consumer(req_cfg, "SmallBASIC");
  
  if (Type == INPUT)
  {
    if (RequestInput) gpiod_line_request_release(RequestInput);
  }
  else if (Type == OUTPUT)
  {
    if (RequestOutput) gpiod_line_request_release(RequestOutput);
  }
  else if (Type == TRIGGER)
  {
    if (RequestTrigger) gpiod_line_request_release(RequestTrigger);
  }

  gpiod_line_request *Request;
  Request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
  if (!Request)
  {
    result = 8;
  }

  if (Type == INPUT)
  {
    RequestInput = Request;
  }
  else if (Type == OUTPUT)
  {
    RequestOutput = Request;
  }
  else if (Type == TRIGGER)
  {
    RequestTrigger = Request;
  }

  gpiod_request_config_free(req_cfg);
free_line_config:
  gpiod_line_config_free(line_cfg);
free_settings:
  gpiod_line_settings_free(settings);
close_chip:
  gpiod_chip_close(chip);

  return(result);
}

int CMD_SetInput(int argc, slib_par_t *params, var_t *retval)
{
  int result = 1;
  unsigned int Pin = get_param_int(argc, params, 0, 0);
  unsigned int Bias = get_param_int(argc, params, 1, 0);
  InputBias = Bias;

  result = RequestLine(Pin, INPUT, -1, Bias);

  if (result)
  {
    char str[20];
    sprintf(str, "SetInput: error %1d", result);
    v_setstr(retval, str);
    return(0);
  }

  return(1);
}

//

int CMD_SetOutput(int argc, slib_par_t *params, var_t *retval)
{
  int result = 1;
  unsigned int Pin = get_param_int(argc, params, 0, 0);

  result = RequestLine(Pin, OUTPUT, -1, -1);

  if (result)
  {
    char str[20];
    sprintf(str, "SetOutput: error %1d", result);
    v_setstr(retval, str);
    return(0);
  }

  return(1);
}

int CMD_SetTrigger(int argc, slib_par_t *params, var_t *retval)
{
  int result = 1;
  unsigned int Pin = get_param_int(argc, params, 0, 0);
  unsigned int EdgeDirection = get_param_int(argc, params, 1, 0);
  unsigned int Bias = get_param_int(argc, params, 2, 0);

  TriggerEdgeDirection = EdgeDirection;
  TriggerBias = Bias;

  result = RequestLine(Pin, TRIGGER, EdgeDirection, Bias);

  if (result)
  {
    char str[20];
    sprintf(str, "SetTrigger: error %1d", result);
    v_setstr(retval, str);
    return(0);
  }

  return(1);
}

int CMD_Write(int argc, slib_par_t *params, var_t *retval)
{
  if(is_param_array(argc, params, 0))
  {
    // Write mutliple pins

    // get pins
    var_p_t PinArray = params[0].var_p;
    if (v_maxdim(PinArray) > 1)
    {
      v_setstr(retval, "Write: 1D pin array required");
      return 0;
    }
    uint32_t NumberPins  = v_ubound(PinArray, 0) - v_lbound(PinArray, 0) + 1;
    unsigned int gpioPins[NumberPins];
    for (uint32_t ii = 0; ii < NumberPins; ii++)
    {
      gpioPins[ii] = get_num(v_elem(PinArray, ii));
    }

    // get values
    enum gpiod_line_value values[NumberPins];

    if (is_param_array(argc, params, 1)) // is array
    {
      var_p_t ValueArray = params[1].var_p;
      if (v_maxdim(ValueArray) > 1)
      {
        v_setstr(retval, "Write: 1D value array required");
        return 0;
      }

      for (uint32_t ii = 0; ii < NumberPins; ii++)
      {
        if (get_num(v_elem(ValueArray, ii))) 
        {
          values[ii] = GPIOD_LINE_VALUE_ACTIVE;
        }
        else
        {
          values[ii] = GPIOD_LINE_VALUE_INACTIVE;
        }
      }
    }
    else // is number
    {
      uint64_t HighLow = get_param_int(argc, params, 1, 0);
      for (uint32_t ii = 0; ii < NumberPins; ii++)
      {
        if ((HighLow >> ii) & 1)
        {
          values[ii] = GPIOD_LINE_VALUE_ACTIVE;
        }
        else
        {
          values[ii] = GPIOD_LINE_VALUE_INACTIVE;
        }
      }
    }

    // Write pins
    int ret = gpiod_line_request_set_values_subset(RequestOutput, NumberPins, gpioPins, values);
    if (ret == -1)
    {
      v_setstr(retval, "Failed to write to pins. Are all pins configured as outputs?");
      return 0;
    }
  }
  else
  {
    // Write single pin

    uint8_t Pin	= get_param_int(argc, params, 0, 0);
    bool HighLow	= get_param_int(argc, params, 1, 0);

    if(Pin > MAX_GPIO_LINES)
    {
      v_setstr(retval, "Max. GPIO pin number is 53.");
      return(0);
    }

    if (PinSettings[Pin] != OUTPUT)
    {
      v_setstr(retval, "GPIO: Pin not configured as output");
      return(0);
    }

    enum gpiod_line_value value = GPIOD_LINE_VALUE_INACTIVE;
    if (HighLow) value = GPIOD_LINE_VALUE_ACTIVE;
    gpiod_line_request_set_value(RequestOutput, Pin, value);
  }

  return(1);
}

int CMD_Read(int argc, slib_par_t *params, var_t *retval)
{
  if(is_param_array(argc, params, 0))
  {
    // Read mutliple pins

    var_p_t array = params[0].var_p;

    if (v_maxdim(array) > 1)
    {
      v_setstr(retval, "Read: 1D-array required");
      return 0;
    }

    uint32_t NumberPins  = v_ubound(array, 0) - v_lbound(array, 0) + 1;
    unsigned int gpioPins[NumberPins];
    for (uint32_t ii = 0; ii < NumberPins; ii++)
    {
      gpioPins[ii] = get_num(v_elem(array, ii));
    }
    enum gpiod_line_value values[NumberPins];
    int ret = gpiod_line_request_get_values_subset(RequestInput, NumberPins, gpioPins, values);
    if (ret == -1)
    {
      v_setstr(retval, "Read: failed to read from pins. Are all pins configured as inputs?");
      return 0;
    }

    v_toarray1(retval, NumberPins);
    uint32_t result;
    for (uint32_t ii = 0; ii < NumberPins; ii++)
    {
      result = 0;
      if (values[ii] == GPIOD_LINE_VALUE_ACTIVE) result = 1;
      v_setint(v_elem(retval, ii), result);
    }
  }
  else
  {
    //Read single pin

    uint8_t Pin = get_param_int(argc, params, 0, 0);
    if (Pin > MAX_GPIO_LINES)
    {
      v_setstr(retval, "Max. GPIO pin number is 53.");
      return(0);
    }

    if (PinSettings[Pin] != INPUT)
    {
      v_setstr(retval, "Read: pin not configured as input");
      return(0);
    }

    uint32_t result = 0;
    enum gpiod_line_value value;
    value = gpiod_line_request_get_value(RequestInput, Pin);
    if (value == GPIOD_LINE_VALUE_ACTIVE) result = 1;
    v_setint(retval, result);
  }

  return(1);
}

int CMD_Trigger(int argc, slib_par_t *params, var_t *retval)
{
  // Emits a trigger pulse with a given pulse length in us
  // and given level. Max. pulse lenght is 65535us. Level is
  // optional. Default value is 1. Pulse lenght is optional.
  // Default value is 50us.

  uint8_t  Pin         = get_param_int(argc, params, 0, 0);
  uint16_t PulseLength = get_param_int(argc, params, 1, 50);
  uint8_t  Level       = get_param_int(argc, params, 2, 1);

  if(Pin > MAX_GPIO_LINES)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if (PinSettings[Pin] != OUTPUT)
  {
    v_setstr(retval, "Trigger: pin not configured as output");
    return(0);
  }

  if (Level)
  {
    gpiod_line_request_set_value(RequestOutput, Pin, GPIOD_LINE_VALUE_ACTIVE);
    usleep(PulseLength);
    gpiod_line_request_set_value(RequestOutput, Pin, GPIOD_LINE_VALUE_INACTIVE);
  }
  else
  {
    gpiod_line_request_set_value(RequestOutput, Pin, GPIOD_LINE_VALUE_INACTIVE);
    usleep(PulseLength);
    gpiod_line_request_set_value(RequestOutput, Pin, GPIOD_LINE_VALUE_ACTIVE);
  }

  return(1);
}

int CMD_WaitEvent(int argc, slib_par_t *params, var_t *retval)
{
  // Wait for a raising edge event at specified pin. Timeout_s is
  // an optional parameter to define a time out in s.
  // Return values:
  //    -1: error
  //     0: time out
  //     1: edge detected

  uint8_t Pin       = get_param_int(argc, params, 0, 0);
  uint8_t Timeout_s = get_param_int(argc, params, 1, 1);

  if(Pin > MAX_GPIO_LINES)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if(PinSettings[Pin] != TRIGGER)
  {
    v_setstr(retval, "WaitTrigger: pin not configured as trigger in");
    return(0);
  }

  int ret;
  int event_buf_size = 1;
  struct gpiod_edge_event_buffer *event_buffer;
  struct gpiod_edge_event *event;

  uint64_t start = millis();

  while (1)
  {
    ret = gpiod_line_request_wait_edge_events(RequestTrigger, Timeout_s * 1E9);
    if (ret < 1)
    {
      v_setint(retval,ret);
      return(1);
    }

    event_buffer = gpiod_edge_event_buffer_new(event_buf_size);
    if (!event_buffer)
    {
      v_setstr(retval, "WaitTrigger: failed to create event buffer");
      return(0);
    }

    ret = gpiod_line_request_read_edge_events(RequestTrigger, event_buffer, event_buf_size);
    if (ret == -1)
    {
      v_setstr(retval, "WaitTrigger: Error reading edge events");
      return(0);
    }

    event = gpiod_edge_event_buffer_get_event(event_buffer, 0);
    if (gpiod_edge_event_get_line_offset(event) == Pin)
    {
      v_setint(retval, 1);
      return(1);
    }

    if (millis() - start > Timeout_s * 1000)
    {
      v_setint(retval, 0);
      return(1);
    }
  }

  return(1);
}

int CMD_ReleasePin(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t Pin = get_param_int(argc, params, 0, 0);
  int ret = 0;
  
  if(Pin > MAX_GPIO_LINES)
  {
    v_setstr(retval, "Max. GPIO pin number is 53.");
    return(0);
  }

  if(PinSettings[Pin] == INPUT)
  {
    PinSettings[Pin] = 0;
    ret = RequestLine(-1, INPUT, -1 , InputBias);
  }
  else if (PinSettings[Pin] == OUTPUT)
  {
    PinSettings[Pin] = 0;
    ret = RequestLine(-1, OUTPUT, -1 , -1);
  }
  else if (PinSettings[Pin] == TRIGGER)
  {
    PinSettings[Pin] = 0;
    ret = RequestLine(-1, TRIGGER, TriggerEdgeDirection , TriggerBias);
  }

  if (ret)
  {
    char str[21];
    sprintf(str, "ReleasePin: Error %1d", ret);
    v_setstr(retval, str);
    return(0);
  }

  return(1);
}