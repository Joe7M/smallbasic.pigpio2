/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT licence, Joerg Siebenmorgen, 2026
*/

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

#include "hd44780.h"
#include "gpio.h"

#define MAX_GPIO_LINES 53

#define LCD_CLEAR_DISPLAY  0x01
#define LCD_CURSOR_HOME    0x02

#define LCD_SET_ENTRY      0x04
#define LCD_ENTRY_DECREASE 0x00
#define LCD_ENTRY_INCREASE 0x02
#define LCD_ENTRY_NOSHIFT  0x00
#define LCD_ENTRY_SHIFT    0x01

#define LCD_SET_DISPLAY    0x08
#define LCD_DISPLAY_OFF    0x00
#define LCD_DISPLAY_ON     0x04
#define LCD_CURSOR_OFF     0x00
#define LCD_CURSOR_ON      0x02
#define LCD_BLINKING_OFF   0x00
#define LCD_BLINKING_ON    0x01

#define LCD_SET_SHIFT      0x10
#define LCD_CURSOR_MOVE    0x00
#define LCD_DISPLAY_SHIFT  0x08
#define LCD_SHIFT_LEFT     0x00
#define LCD_SHIFT_RIGHT    0x04

#define LCD_SET_FUNCTION   0x20
#define LCD_FUNCTION_4BIT  0x00
#define LCD_FUNCTION_8BIT  0x10
#define LCD_FUNCTION_1LINE 0x00
#define LCD_FUNCTION_2LINE 0x08
#define LCD_FUNCTION_5X7   0x00
#define LCD_FUNCTION_5X10  0x04

#define LCD_SOFT_RESET     0x30

#define LCD_SET_CGADR      0x40
#define LCD_SET_DDADR      0x80
#define LCD_DDADR_LINE1    0x00
#define LCD_DDADR_LINE2    0x40
#define LCD_DDADR_LINE3    0x10
#define LCD_DDADR_LINE4    0x50

uint8_t Pin_DB4, Pin_DB5, Pin_DB6, Pin_DB7, Pin_E, Pin_RS;

void DelayMilli(uint32_t ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;

    nanosleep(&ts, &ts);
}

void DelayMicro(uint32_t us)
{
    struct timespec ts;

    ts.tv_sec = us / 1000000;
    ts.tv_nsec = (us % 1000000) * 1000;

    nanosleep(&ts, &ts);
}

void SendEnable(void)
{
  GPIO_Write(Pin_E, 1);
  DelayMicro(100);
  GPIO_Write(Pin_E, 0);
  DelayMicro(100);
}

void Send4Bit(uint8_t b)
{
  GPIO_Write(Pin_DB7, (b >> 7) & 1);
  GPIO_Write(Pin_DB6, (b >> 6) & 1);
  GPIO_Write(Pin_DB5, (b >> 5) & 1);
  GPIO_Write(Pin_DB4, (b >> 4) & 1);
  SendEnable();
}

void SendByte(uint8_t b)
{
  Send4Bit(b);
  Send4Bit(b << 4);
}

void SendCommand(uint8_t cmd)
{
  GPIO_Write(Pin_RS, 0);
  SendByte(cmd);
}

void SendData(uint8_t dat)
{
  GPIO_Write(Pin_RS, 1);
  SendByte(dat);
}

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  Pin_DB4 = get_param_int(argc, params, 0, 0);    // Pin 27
  Pin_DB5 = get_param_int(argc, params, 1, 5);    // Pin 29
  Pin_DB6 = get_param_int(argc, params, 2, 6);    // Pin 31
  Pin_DB7 = get_param_int(argc, params, 3, 13);   // Pin 33
  Pin_E   = get_param_int(argc, params, 4, 19);   // Pin 35
  Pin_RS  = get_param_int(argc, params, 5, 26);   // Pin 37
  const char *text = get_param_str(argc, params, 6, "/dev/gpiochip0");
  char *gpiochipname = new char[strlen(text) + 2];
  strcpy(gpiochipname, text);

  // Init gpio
  if(Pin_DB4 > MAX_GPIO_LINES)
  {
    v_setstr(retval, "DB4 pin number out of range.");
    return(0);
  }
  if(Pin_DB5 > MAX_GPIO_LINES)
  {
    v_setstr(retval, "DB5 pin number out of range.");
    return(0);
  }
  if(Pin_DB6 > MAX_GPIO_LINES)
  {
    v_setstr(retval, "DB6 pin number out of range.");
    return(0);
  }
  if(Pin_DB7 > MAX_GPIO_LINES)
  {
    v_setstr(retval, "DB7 pin number out of range.");
    return(0);
  }
  if(Pin_E > MAX_GPIO_LINES)
  {
    v_setstr(retval, "E pin number out of range.");
    return(0);
  }
  if(Pin_RS > MAX_GPIO_LINES)
  {
    v_setstr(retval, "RS pin number out of range.");
    return(0);
  }

  if(GPIO_SetOutput(gpiochipname, Pin_E, Pin_RS, Pin_DB4, Pin_DB5, Pin_DB6, Pin_DB7))
  {
    v_setstr(retval, "Error setting pins to output");
    return(0);
  }

  // Hardware reset
  GPIO_Write(Pin_E, 0);
  GPIO_Write(Pin_RS, 0);
  GPIO_Write(Pin_DB7, 0);
  GPIO_Write(Pin_DB6, 0);
  GPIO_Write(Pin_DB5, 0);
  GPIO_Write(Pin_DB4, 0);
  DelayMilli(50);

  // Send soft-reset 3 time to initialize LCD
  Send4Bit(LCD_SOFT_RESET);
  DelayMilli(5);
  SendEnable();
  DelayMilli(1);
  SendEnable();
  DelayMilli(1);

  // Set 4-bit mode
  Send4Bit(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT);
  DelayMilli(5);

  // 2 lines and 5x7 pixel in 4 bit mode
  SendCommand(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINE | LCD_FUNCTION_5X7);
  // Display on, cursor off and blinking off
  SendCommand(LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINKING_OFF);
  // Cursor increment no scrolling
  SendCommand(LCD_SET_ENTRY | LCD_ENTRY_INCREASE | LCD_ENTRY_NOSHIFT);
  // Clear screen and set cursor
  SendCommand(LCD_CLEAR_DISPLAY);
  SendCommand(LCD_CURSOR_HOME);

  return(1);
}

int CMD_Print(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "");

  for(uint32_t ii = 0; ii < (uint32_t)strlen(text); ii++)
  {
    SendData(text[ii]);
  }

  return(1);
}

int CMD_Cls(int argc, slib_par_t *params, var_t *retval)
{
  SendCommand(LCD_CLEAR_DISPLAY);
  SendCommand(LCD_CURSOR_HOME);
  return(1);
}

int CMD_Off(int argc, slib_par_t *params, var_t *retval)
{
  SendCommand(LCD_SET_DISPLAY | LCD_DISPLAY_OFF);
  return(1);
}

int CMD_On(int argc, slib_par_t *params, var_t *retval)
{
  SendCommand(LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINKING_OFF);
  return(1);
}

int CMD_Locate(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t x = get_param_int(argc, params, 0, 1);
  uint8_t y = get_param_int(argc, params, 1, 1);
  uint8_t dat;

  if(x < 1) x = 1;

  switch(y)
  {
    case 1:     // 1. line
      dat = LCD_SET_DDADR + LCD_DDADR_LINE1 + x - 1;
      break;
    case 2:     // 2. line
      dat = LCD_SET_DDADR + LCD_DDADR_LINE2 + x - 1;
      break;
    case 3:     // 3. line
      dat = LCD_SET_DDADR + LCD_DDADR_LINE3 + x - 1;
      break;
    case 4:     // 4. line
      dat = LCD_SET_DDADR + LCD_DDADR_LINE4 + x - 1;
      break;
    default:
      v_setstr(retval, "Locate: y value must be smaller than 5");
      return(0);
  }

  SendCommand(dat);
  return(1);
}

int Close(void)
{
  GPIO_Close();
  return(1);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  Close();
  return(1);
}
