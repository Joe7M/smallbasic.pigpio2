//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#ifndef _GPIO_H_
#define _GPIO_H_

#include <linux/i2c-dev.h>
#include <gpiod.h>
#include <stdint.h>

int GPIO_Open(char *gpiochipname, struct gpiod_chip **gpiochip)
{
  *gpiochip = gpiod_chip_open_by_name(gpiochipname);
  if(*gpiochip == NULL ) return(1);
  return(0);
}

int GPIO_SetOutput(struct gpiod_chip **gpiochip, struct gpiod_line **gpiolines, uint8_t Pin)
{
  if(gpiolines[Pin] != NULL) gpiod_line_release(gpiolines[Pin]);

  gpiolines[Pin] = gpiod_chip_get_line(*gpiochip, Pin);
  if(gpiolines[Pin] == NULL) return(1);

  if(gpiod_line_request_output(gpiolines[Pin], "SmallBasicPIGPIO", 0) == -1)
  {
    gpiod_line_release(gpiolines[Pin]);
    return(1);
  }

  return(0);
}

int GPIO_Write(struct gpiod_line **gpiolines, uint8_t Pin, bool HighLow)
{
  gpiod_line_set_value(gpiolines[Pin], HighLow);
  return(0);
}

#endif /* !_GPIO_H_ */
