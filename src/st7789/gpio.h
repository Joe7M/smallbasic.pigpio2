//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#ifndef _GPIO_H_
#define _GPIO_H_

#include <linux/i2c-dev.h>
#include <gpiod.h>
#include <stdint.h>

int GPIO_Open(char *gpiochipname, struct gpiod_chip **gpiochip);
int GPIO_SetOutput(struct gpiod_chip **gpiochip, struct gpiod_line **gpiolines, uint8_t Pin);
int GPIO_Write(struct gpiod_line **gpiolines, uint8_t Pin, bool HighLow);

#endif /* !_GPIO_H_ */
