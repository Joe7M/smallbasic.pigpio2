/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT licence, Joerg Siebenmorgen, 2026
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <linux/i2c-dev.h>
#include <gpiod.h>
#include <stdint.h>

int GPIO_SetOutput(char *Chipname, unsigned int Pin_E, unsigned int Pin_RS, unsigned int Pin_DB4, unsigned int Pin_DB5,unsigned int Pin_DB6, unsigned int Pin_DB7);
int GPIO_Write(unsigned int Pin, unsigned int HighLow);
int GPIO_Close(void);

#endif /* !_GPIO_H_ */
