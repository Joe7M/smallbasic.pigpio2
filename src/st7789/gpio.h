//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#ifndef _GPIO_H_
#define _GPIO_H_

#include <linux/i2c-dev.h>
#include <gpiod.h>
#include <stdint.h>

int GPIO_SetOutput(char *Chipname, unsigned int Pin_RST, unsigned int Pin_DC, unsigned int Pin_BL);
int GPIO_Write(unsigned int Pin, unsigned int HighLow);
int GPIO_Close(void);

#endif /* !_GPIO_H_ */
