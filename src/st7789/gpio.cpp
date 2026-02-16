//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#ifndef _GPIO_H_
#define _GPIO_H_

#include <linux/i2c-dev.h>
#include <gpiod.h>
#include <stdint.h>
#include <stdio.h>

struct gpiod_line_request *RequestOutput = NULL;

int GPIO_SetOutput(char *chipname, unsigned int Pin_RST, unsigned int Pin_DC, unsigned int Pin_BL)
{
  struct gpiod_request_config *req_cfg = NULL;
  struct gpiod_line_settings *settings;
  struct gpiod_line_config *line_cfg;
  struct gpiod_chip *chip;
  int ret, result = 0;

  chip = gpiod_chip_open(chipname);
  if (!chip) return(3);

  settings = gpiod_line_settings_new();
  if (!settings)
  {
    result = 4;
    goto close_chip;
  }

  gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
  gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

  line_cfg = gpiod_line_config_new();
  if (!line_cfg)
  {
    result = 5;
    goto free_settings;
  }

  static const unsigned int Pins[3] = {Pin_RST, Pin_DC, Pin_BL};
  ret = gpiod_line_config_add_line_settings(line_cfg, Pins, 3, settings);
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

  RequestOutput = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
  if (!RequestOutput)
  {
    result = 8;
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

int GPIO_Write(unsigned int Pin, unsigned int HighLow)
{
  enum gpiod_line_value value;

  if (HighLow)
  {
    value = GPIOD_LINE_VALUE_ACTIVE;
  }
  else
  {
    value = GPIOD_LINE_VALUE_INACTIVE;
  }

  if (gpiod_line_request_set_value(RequestOutput, Pin, value) == -1)
  {
    return 1;
  }
  return(0);
}

int GPIO_Close(void)
{
  if (RequestOutput) gpiod_line_request_release(RequestOutput);
  RequestOutput = NULL;
  return(1);
}

#endif /* !_GPIO_H_ */
