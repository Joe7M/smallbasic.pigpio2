
// This file is part of the SmallBasicPIGPIO plugin to
// use gpio pins on a Raspberry pi with SmallBASIC.
// MIT licence
// Joerg Siebenmorgen, 2024
//
// SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
// Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

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

#include <linux/spi/spidev.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <gpiod.h>
#include <time.h>

#include "gpio.h"
#include "spi.h"
#include "st7789.h"
#include "../framebuffer/FBdraw.hpp"

#define _swap(a, b) { int16_t t = a; a = b; b = t; }

#define MAX_GPIO_LINE 53

#define ST7789_NOP            0x00
#define ST7789_SWRESET        0x01
#define ST7789_SLPOUT         0x11
#define ST7789_NORON          0x13
#define ST7789_INVON          0x21
#define ST7789_DISPON         0x29
#define ST7789_CASET          0x2A
#define ST7789_RASET          0x2B
#define ST7789_RAMWR          0x2C
#define ST7789_COLMOD         0x3A
#define ST7789_MADCTL         0x36
#define ST7789_MADCTL_MY      0x80
#define ST7789_MADCTL_MX      0x40
#define ST7789_MADCTL_MV      0x20
#define ST7789_MADCTL_ML      0x10
#define ST7789_MADCTL_RGB     0x00
#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 0
#define ST7789_TFTWIDTH       240
#define ST7789_TFTHEIGHT      240

uint8_t Pin_RST;
uint8_t Pin_DC;
uint8_t Pin_BL;

int fd_spi = 0;
uint32_t PageSize = 4096;

uint32_t colstart = 0;
uint32_t rowstart = 0;
uint32_t ystart   = 0;
uint32_t xstart   = 0;

int16_t LCDWIDTH  = 240;
int16_t LCDHEIGHT = 240;
FBDraw fbdraw;

void _Command(uint8_t command);
void _Data(uint8_t data);

uint8_t* Framebuffer;


void msleep(uint32_t msec)
{
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    nanosleep(&ts, &ts);
}

void WriteCmd(uint8_t command)
{
  uint8_t buffer[1];
  buffer[0] = command;
  GPIO_Write(Pin_DC, 0);
  SPI_Write(fd_spi, buffer, 1, PageSize);
}

void WriteData8(uint8_t data)
{
  uint8_t buffer[1];
  buffer[0] = data;
  GPIO_Write(Pin_DC, 1);
  SPI_Write(fd_spi, buffer, 1, PageSize);
}

void WriteData16(uint16_t data)
{
  uint8_t buffer[2];
  buffer[0] = (uint8_t) (data >> 8);
  buffer[1] = (uint8_t) (data & 0xFF);
  GPIO_Write(Pin_DC, 1);
  SPI_Write(fd_spi, buffer, 2, PageSize);
}

void SetAddrWindow(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye)
{
  xs += xstart;
  xe += xstart;
  ys += ystart;
  ye += ystart;

  // CASET
  WriteCmd(ST7789_CASET);
  GPIO_Write(Pin_DC, 1);
  uint8_t buffer[2];

  buffer[0] = xs >> 8;
  buffer[1] = xs & 0xFF;
  SPI_Write(fd_spi, buffer, 2, PageSize);
  buffer[0] = xe >> 8;
  buffer[1] = xe & 0xFF;
  SPI_Write(fd_spi, buffer, 2, PageSize);

  // RASET
  WriteCmd(ST7789_RASET);
  GPIO_Write(Pin_DC, 1);
  buffer[0] = ys >> 8;
  buffer[1] = ys & 0xFF;
  SPI_Write(fd_spi, buffer, 2, PageSize);
  buffer[0] = ye >> 8;
  buffer[1] = ye & 0xFF;
  SPI_Write(fd_spi, buffer, 2, PageSize);
}

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text1 = get_param_str(argc, params, 2, "/dev/spidev0.0");
  char *device = new char[strlen(text1) + 2];
  strcpy(device,text1);

  PageSize = get_param_int(argc, params, 8, 4096);

	// Init spi
  fd_spi = SPI_Open(device);
  if (fd_spi < 0)
  {
    v_setstr(retval, "Error opening SPI device");
    SPI_Close(fd_spi);
    return(0);
  }

  uint32_t Speed = get_param_int(argc, params, 7, 10000000);
  if(SPI_SetSpeed(fd_spi, Speed) != 0)
  {
    v_setstr(retval, "Error setting SPI speed");
    SPI_Close(fd_spi);
    return(0);
  }

  if(SPI_SetMSBFirst(fd_spi) != 0)
  {
    v_setstr(retval, "Error setting MSB first");
    SPI_Close(fd_spi);
    return(0);
  }

  if(SPI_SetBitsPerWord(fd_spi, 8) != 0)
  {
    v_setstr(retval, "Error writing bits per word");
    SPI_Close(fd_spi);
    return(0);
  }

  if(SPI_SetMode(fd_spi, SPI_MODE_0) != 0)
  {
    v_setstr(retval, "Error writing SPI mode");
    SPI_Close(fd_spi);
    return(0);
  }

  // Init gpio
  const char *text2 = get_param_str(argc, params, 3, "/dev/gpiochip0");
  char *gpiochipname = new char[strlen(text2) + 2];
  strcpy(gpiochipname, text2);

  Pin_RST = get_param_int(argc, params, 4, 27);
  Pin_DC  = get_param_int(argc, params, 5, 17);
  Pin_BL  = get_param_int(argc, params, 6, 22);

  if(Pin_RST > MAX_GPIO_LINE)
  {
    v_setstr(retval, "RST pin number out of range.");
    SPI_Close(fd_spi);
    return(0);
  }
  if(Pin_DC > MAX_GPIO_LINE)
  {
    v_setstr(retval, "DC pin number out of range.");
    SPI_Close(fd_spi);
    return(0);
  }
  if(Pin_BL > MAX_GPIO_LINE)
  {
    v_setstr(retval, "BL pin number out of range.");
    SPI_Close(fd_spi);
    return(0);
  }

  if(GPIO_SetOutput(gpiochipname, Pin_RST, Pin_DC, Pin_BL))
  {
    v_setstr(retval, "Error setting RST, DC, BL pin to output");
    SPI_Close(fd_spi);
    return(0);
  }

  LCDWIDTH  = get_param_int(argc, params, 0, 240);
  LCDHEIGHT = get_param_int(argc, params, 1, 240);

  if(LCDWIDTH == 240 && LCDHEIGHT == 240) rowstart = 80;

  // Background light on
  GPIO_Write(Pin_BL, 1);

  // Hardware reset
  GPIO_Write(Pin_RST, 1);
  msleep(50);
  GPIO_Write(Pin_RST, 0);
  msleep(50);
  GPIO_Write(Pin_RST, 1);
  msleep(150);

  // Init
  WriteCmd(ST7789_SWRESET); msleep(150);
  WriteCmd(ST7789_SLPOUT);  msleep(500);
  WriteCmd(ST7789_COLMOD);  WriteData8(0x55); msleep(10);     // RGB565
  WriteCmd(ST7789_MADCTL);  WriteData8(0x0);
  WriteCmd(ST7789_CASET);   WriteData16(0); WriteData16(LCDWIDTH);
  WriteCmd(ST7789_RASET);   WriteData16(0); WriteData16(LCDHEIGHT);
  WriteCmd(ST7789_INVON);   msleep(10);
  WriteCmd(ST7789_NORON);   msleep(10);
  WriteCmd(ST7789_DISPON);  msleep(10);

  Framebuffer = new uint8_t[2 * LCDWIDTH * LCDHEIGHT];

  fbdraw.Init(LCDWIDTH, LCDHEIGHT, 16, Framebuffer);

  return(1);
}

void Close(void)
{
  SPI_Close(fd_spi);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  Close();
  return(1);
}

int CMD_Display(int argc, slib_par_t *params, var_t *retval)
{
  //dump screen
  //for(uint32_t ii = 0 ; ii < LCDHEIGHT * LCDWIDTH; ii+=2)
  //    printf("0x%04x, ", (Framebuffer[ii] << 8) + Framebuffer[ii+1]);

//	SetAddrWindow(0, 0, LCDWIDTH, LCDHEIGHT);
  WriteCmd(ST7789_RAMWR);
  GPIO_Write(Pin_DC, 1);
  SPI_Write(fd_spi, Framebuffer, 2*LCDWIDTH*LCDHEIGHT, PageSize);

  return(1);
}


int CMD_ClearDisplay(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t color = get_param_int(argc, params, 0, fbdraw.GetBGColor());
  fbdraw.Clear(color);
  return(1);
}

int CMD_Pset(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x = get_param_int(argc, params, 0, 0);
  uint16_t y = get_param_int(argc, params, 1, 0);
  uint16_t color = get_param_int(argc, params, 2, fbdraw.GetFGColor());
  fbdraw.Pixel(x, y, color);
  return(1);
}


int CMD_Line(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x1 = get_param_int(argc, params, 0, 0);
  uint16_t y1 = get_param_int(argc, params, 1, 0);
  uint16_t x2 = get_param_int(argc, params, 2, 0);
  uint16_t y2 = get_param_int(argc, params, 3, 0);
  uint16_t color = get_param_int(argc, params, 4, fbdraw.GetFGColor());
  fbdraw.Line(x1, y1, x2, y2, color);
  return(1);
}

int CMD_Rect(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x1 = get_param_int(argc, params, 0, 0);
  uint16_t y1 = get_param_int(argc, params, 1, 0);
  uint16_t w = get_param_int(argc, params, 2, 0);
  uint16_t h = get_param_int(argc, params, 3, 0);
  uint16_t color = get_param_int(argc, params, 4, fbdraw.GetFGColor());
  uint8_t filled = get_param_int(argc, params, 5, 0);

  if(filled)
    fbdraw.RectFill(x1, y1, x1 + w, y1 + h, color);
  else
    fbdraw.Rect(x1, y1, x1 + w, y1 + h, color);

  return(1);
}

int CMD_Circle(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x = get_param_int(argc, params, 0, 0);
  uint16_t y = get_param_int(argc, params, 1, 0);
  uint16_t r = get_param_int(argc, params, 2, 0);
  uint16_t color = get_param_int(argc, params, 3, fbdraw.GetFGColor());
  uint8_t filled = get_param_int(argc, params, 4, 0);

  if(filled)
    fbdraw.CircleFill(x, y, r, color);
  else
    fbdraw.Circle(x, y, r, color);

  return(1);
}

int CMD_RoundRect(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x1 = get_param_int(argc, params, 0, 0);
  uint16_t y1 = get_param_int(argc, params, 1, 0);
  uint16_t w = get_param_int(argc, params, 2, 0);
  uint16_t h = get_param_int(argc, params, 3, 0);
  uint16_t r = get_param_int(argc, params, 4, 3);
  uint16_t color = get_param_int(argc, params, 5, fbdraw.GetFGColor());
  uint8_t filled = get_param_int(argc, params, 6, 0);

  if(filled)
    fbdraw.RoundRectFill(x1, y1, x1 + w, y1 + h, r, color);
  else
    fbdraw.RoundRect(x1, y1, x1 + w, y1 + h, r, color);

  return(1);
}

int CMD_Triangle(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x1 = get_param_int(argc, params, 0, 0);
  uint16_t y1 = get_param_int(argc, params, 1, 0);
  uint16_t x2 = get_param_int(argc, params, 2, 0);
  uint16_t y2 = get_param_int(argc, params, 3, 0);
  uint16_t x3 = get_param_int(argc, params, 4, 0);
  uint16_t y3 = get_param_int(argc, params, 5, 0);
  uint16_t color = get_param_int(argc, params, 6, fbdraw.GetFGColor());
  uint8_t filled = get_param_int(argc, params, 7, 0);

  if(filled)
    fbdraw.TriangleFill(x1, y1, x2, y2, x3, y3, color);
  else
    fbdraw.Triangle(x1, y1, x2, y2, x3, y3, color);

  return(1);
}

int CMD_Color(int argc, slib_par_t *params, var_t *retval)
{
  fbdraw.SetFGColor(get_param_int(argc, params, 0, 0));
  fbdraw.SetBGColor(get_param_int(argc, params, 1, fbdraw.GetBGColor()));

  return(1);
}


int CMD_Print(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, NULL);
  char *text1 = new char[strlen(text) + 2];
  strcpy(text1,text);
  strcat(text1,"\n");
  uint16_t color = get_param_int(argc, params, 1, fbdraw.GetFGColor());

  fbdraw.Text(text1, color);

  return(1);
}

int CMD_SetTextSize(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t TextSize = get_param_int(argc, params, 0, 8);
  fbdraw.SetTextSize(TextSize);
  return(1);
}

int CMD_At(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x = get_param_int(argc, params, 0, 0);
  uint16_t y = get_param_int(argc, params, 1, 0);
  fbdraw.GotoXY(x, y);
  return(1);
}

int CMD_CopyToDisplay(int argc, slib_par_t *params, var_t *retval)
{
  if(!is_param_array(argc, params, 0))
  {
    v_setstr(retval, "BitBlt requires an 2D-array");
    return 0;
  }

  var_p_t array = params[0].var_p;  //Get array

  if(v_maxdim(array) != 2)
  {
    v_setstr(retval, "BitBlt requires an 2D-array");
    return 0;
  }

  uint16_t SizeX = v_ubound(array, 0) - v_lbound(array, 0) + 1;
  uint16_t SizeY = v_ubound(array, 1) - v_lbound(array, 1) + 1;

  if(SizeX > LCDWIDTH || SizeY > LCDHEIGHT)
  {
    v_setstr(retval, "SSD1306: BitBlt requires an 2D-array with maximal dimensions of the OLED.");
    return 0;
  }

  int32_t PosX = get_param_int(argc, params, 1, 0);  //Get pos x
  int32_t PosY = get_param_int(argc, params, 2, 0);  //Get pos y

  uint16_t DrawSizeX = SizeX;
  uint16_t DrawSizeY = SizeY;

  if(LCDWIDTH - PosX < SizeX) DrawSizeX = LCDWIDTH - PosX;
  if(LCDHEIGHT - PosY < SizeY) DrawSizeY = LCDHEIGHT - PosY;

  uint8_t BitBltMode = get_param_int(argc, params, 3, 0);  //Get mode

  switch(BitBltMode)
  {
    case 1: //Black is transparent; Value = 0 -> transparent; Value > 0 -> white
      for(uint32_t xx = 0; xx < DrawSizeX; xx++)
      {
        for(uint32_t yy = 0; yy < DrawSizeY; yy++)
        {
          if(get_array_elem_num(array, xx * SizeY + yy))
          {
            Framebuffer[xx + PosX + (yy+PosY)*LCDWIDTH] = 1;
          }
        }
      }
      break;
    case 2: //White is transparent; Value = 0 -> black; Value > 0 -> transparent
      for(uint32_t xx = 0; xx < DrawSizeX; xx++)
      {
        for(uint32_t yy = 0; yy < DrawSizeY; yy++)
        {
          if(!get_array_elem_num(array, xx * SizeY + yy))
          {
            Framebuffer[xx + PosX + (yy+PosY)*LCDWIDTH] = 0;
          }
        }
      }
      break;
    case 3: // Value > 1 and Value < 0xFF are transparent
            // Value = 0 -> black; Value = 0xFF -> white
      for(uint32_t xx = 0; xx < DrawSizeX; xx++)
      {
        for(uint32_t yy = 0; yy < DrawSizeY; yy++)
        {
          if(get_array_elem_num(array, xx * SizeY + yy) == 0) // Black
            Framebuffer[xx + PosX + (yy+PosY)*LCDWIDTH] = 0;
          else if(get_array_elem_num(array, xx * SizeY + yy) == 0xFF) // White
            Framebuffer[xx + PosX + (yy+PosY)*LCDWIDTH] = 1;
        }
      }
      break;
    default: //No transparancy; Value = 0 -> black; Value > 0 -> white
      for(uint32_t xx = 0; xx < DrawSizeX; xx++)
      {
        for(uint32_t yy = 0; yy < DrawSizeY; yy++)
        {
          fbdraw.Pixel(xx + PosX, yy + PosY, !!get_array_elem_num(array, xx * SizeY + yy));
        }
      }
  }
  return(1);
}

int CMD_CopyFromDisplay(int argc, slib_par_t *params, var_t *retval)
{
  int16_t x = get_param_int(argc, params, 0, 0);
  int16_t y = get_param_int(argc, params, 1, 0);
  uint16_t w = get_param_int(argc, params, 2, LCDWIDTH - 1);
  uint16_t h = get_param_int(argc, params, 3, LCDHEIGHT - 1);

  if(x >= LCDWIDTH || y >= LCDHEIGHT) return(1);
  if(x + w > LCDWIDTH) w = LCDWIDTH - x;
  if(y + h > LCDHEIGHT) h = LCDHEIGHT - y;

  v_tomatrix(retval, w, h);

  uint32_t ii = 0;
  for(int16_t xx = x; xx < x + w; xx++)
  {
    for(int16_t yy = y; yy < y + h; yy++)
    {
      v_setreal(v_elem(retval, ii),  255*Framebuffer[xx + yy*LCDWIDTH]);
      ii++;
    }
  }
  return(1);
}
