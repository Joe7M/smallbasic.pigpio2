
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
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ssd1306.h"
#include "../framebuffer/FBdraw.hpp"

#define SSD1306_swap(a, b) { int16_t t = a; a = b; b = t; }

int fd = 0;
int16_t SSD1306_LCDWIDTH = 128;
int16_t SSD1306_LCDHEIGHT = 64;
int16_t InvertDisplay = false;
FBDraw fbdraw;

void SSD1306_Command(uint8_t command);

uint8_t* Framebuffer;

static uint8_t SSD1306_CommandGraphicsBuffer[128 * 64 / 8 + 1] = {
0x40, //To send the whole buffer in one go, the first byte is the ssd1306-command 'send data'
      // the rest of the buffer is the graphics buffer
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0xe0,
0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xc0,
0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0,
0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0xe0,
0xe0, 0xe0, 0xe0, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x67, 0x67, 0x67, 0x66, 0x66, 0x7e, 0x7e, 0x7e, 0x3c, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x06, 0x06, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x60,
0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x66, 0x66, 0x7f, 0x7f, 0x7f, 0x39, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x06,
0x06, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x63, 0x67, 0x67, 0x67, 0x66, 0x66, 0x7e, 0x7e, 0x7e, 0x3c, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x3f, 0x7f, 0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6,
0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38,
0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6,
0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38,
0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6,
0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0x00, 0xfe, 0x22, 0x22, 0x22, 0x1c, 0x00, 0xfa, 0x00, 0x7c, 0x82,
0x92, 0x92, 0x74, 0x00, 0xfe, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x82, 0xfe, 0x82, 0x00, 0x7c, 0x82, 0x82, 0x82,
0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x08, 0xe8, 0xe8, 0xe8, 0x08, 0xf8, 0x00, 0x50, 0x30, 0x38, 0xa0,
0x00, 0x48, 0x70, 0xd0, 0xd8, 0x00, 0xf8, 0x08, 0xe8, 0xe8, 0xe8, 0x08, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xab, 0x8a, 0x4a, 0x8a, 0xda, 0x12, 0xab, 0x08, 0x2b, 0xed,
0x07, 0x10, 0xf3, 0x98, 0xae, 0x21, 0x67, 0x88, 0xc3, 0xda, 0x82, 0x4a, 0x82, 0xea, 0x23, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xef, 0x20, 0xa1, 0xa9, 0xaf, 0x20, 0xea, 0x0f,
0xbf, 0x28, 0x52, 0x95, 0xcd, 0xa1, 0x8a, 0x96, 0xfb, 0x8f, 0xae, 0x8b, 0xfc, 0xc8, 0x9a, 0x22, 0x22, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x08, 0x0b, 0x0b, 0x0b, 0x08,
0x0f, 0x00, 0x0f, 0x05, 0x0e, 0x05, 0x04, 0x08, 0x0c, 0x0d, 0x07, 0x01, 0x05, 0x0e, 0x05, 0x0a, 0x0b, 0x09,
0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t *SSD1306_GraphicsBuffer = &SSD1306_CommandGraphicsBuffer[1];



void SSD1306_Command(uint8_t command)
{
  uint8_t input_buffer[2];
  input_buffer[0] = 0x00;   // Next Byte is a command
  input_buffer[1] = command;
  write(fd, input_buffer, 2);
}

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "/dev/i2c-1");
  char *device = new char[strlen(text) + 2];
  strcpy(device,text);
  uint8_t Address = get_param_int(argc, params, 1, 0x3C);
  SSD1306_LCDWIDTH = get_param_int(argc, params, 2, 128);
  SSD1306_LCDHEIGHT = get_param_int(argc, params, 3, 64);

  if(SSD1306_LCDWIDTH != 128 || (SSD1306_LCDHEIGHT != 64 && SSD1306_LCDHEIGHT != 32))
{
    v_setstr(retval, "SSD1306: Only OLED with 128x64 or 128x32 is supported");
    return 0;
  }

  fd = open(device, O_RDWR);
  if (fd < 0)
  {
    v_setstr(retval, "Error opening I2C bus");
    return(0);
  }
  auto rc = ioctl(fd, I2C_SLAVE, Address);
  if (rc < 0)
  {
    v_setstr(retval, "Error opening SSD1306");
    close(fd);
    return(0);
  }

  // Init sequence according to specsheet

  // Display off
  SSD1306_Command(0xAE);

  // Set Multiplex Ratio
  SSD1306_Command(0xA8);
  SSD1306_Command(SSD1306_LCDHEIGHT - 1);

  // Display Offset
  SSD1306_Command(0xD3);
  SSD1306_Command(0x0);

  // Set Display Clock Divide Ratio / Oscillator Frequency
  // suggested ratio 0x80
  SSD1306_Command(0xD5);
  SSD1306_Command(0x80);

  // Display Start Line -> 0
  SSD1306_Command(0x40);

  // Set Segment Re-map: column address 127 is mapped to SEG0
  SSD1306_Command(0xA1);

  // Set COM Output Scan Direction: remapped mode. Scan from COM[N-1] to COM0
  SSD1306_Command(0xC8);

  //  Set COM Pins Hardware Configuration
  //  Alternative COM pin configuration + Disable COM Left/Right remap
  SSD1306_Command(0xDA);
  if(SSD1306_LCDHEIGHT == 64)
    SSD1306_Command(0x12);
  else
    SSD1306_Command(0x02);

  // Set Contrast Control (Brightness)
  // 0 to 255
  SSD1306_Command(0x81);
  SSD1306_Command(200);

  // Entire Display ON: A4h command enable display outputs according to the GDDRAM contents
  SSD1306_Command(0xA4);

  // Set Normal Display: This command sets the display to be either normal or inverse.
  // In normal display a RAM data of 1 indicates an  “ON” pixel while in inverse display
  // a RAM data of 0 indicates an “ON” pixel
  SSD1306_Command(0xA6);

  // Charge Pump Setting
  // Enable Charge Pump
  SSD1306_Command(0x8D);
  SSD1306_Command(0x14);

  // Memory Addressing Mode
  // Horizontal Addressing Mode
  SSD1306_Command(0x20);
  SSD1306_Command(0x00);
  
  if(SSD1306_LCDHEIGHT == 32)
  {
    SSD1306_Command(0x22);
    SSD1306_Command(0x0);
    SSD1306_Command(0x03);
  }
  
  // turn on display
  SSD1306_Command(0xAF);

  Framebuffer = new uint8_t[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT];

  fbdraw.Init(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, 8, Framebuffer);

  if(write(fd, SSD1306_CommandGraphicsBuffer, SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8 + 1) < 0)
  {
    v_setstr(retval, "Error while writing to device.");
    return(0);
  }

  return(1);
}

void Close(void)
{
  SSD1306_Command(0xAE);    // Display off
  close(fd);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  SSD1306_Command(0xAE);    // Display off
  close(fd);
  return(1);
}

int CMD_Display(int argc, slib_par_t *params, var_t *retval)
{
  CopyFramebuffer(Framebuffer, SSD1306_GraphicsBuffer);

  //dump screen
  //for(uint32_t ii = 0 ; ii < SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8; ii++)
  //    printf("0x%02x, ", SSD1306_GraphicsBuffer[ii]);

  if(write(fd, SSD1306_CommandGraphicsBuffer, SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8 + 1) < 0)
  {
    v_setstr(retval, "Error while writing to device.");
    return(0);
  }
  return(1);
}


int CMD_ClearDisplay(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t color = get_param_int(argc, params, 0, 0);
  fbdraw.Clear(color);
  return(1);
}

int CMD_SetBrightness(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t Brightness = get_param_int(argc, params, 0, 255);
  SSD1306_Command(0x81);
  SSD1306_Command(Brightness);
  return(1);
}


int CMD_InvertDisplay(int argc, slib_par_t *params, var_t *retval)
{
  InvertDisplay = !InvertDisplay;
  if(InvertDisplay)
    SSD1306_Command(0xA7);    //inverted display
  else
    SSD1306_Command(0xA6);     //normal display
  return(1);
}

int CMD_DisplayOn(int argc, slib_par_t *params, var_t *retval)
{
  SSD1306_Command(0xAF);
  return(1);
}

int CMD_DisplayOff(int argc, slib_par_t *params, var_t *retval)
{
  SSD1306_Command(0xAE);
  return(1);
}

int CMD_Pset(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x = get_param_int(argc, params, 0, 0);
  uint16_t y = get_param_int(argc, params, 1, 0);
  uint8_t color = get_param_int(argc, params, 2, 1);
  fbdraw.Pixel(x, y, color);
  return(1);
}


int CMD_Line(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x1 = get_param_int(argc, params, 0, 0);
  uint16_t y1 = get_param_int(argc, params, 1, 0);
  uint16_t x2 = get_param_int(argc, params, 2, 0);
  uint16_t y2 = get_param_int(argc, params, 3, 0);
  uint8_t color = get_param_int(argc, params, 4, 1);
  fbdraw.Line(x1, y1, x2, y2, color);
  return(1);
}

int CMD_Rect(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x1 = get_param_int(argc, params, 0, 0);
  uint16_t y1 = get_param_int(argc, params, 1, 0);
  uint16_t x2 = get_param_int(argc, params, 2, 0);
  uint16_t y2 = get_param_int(argc, params, 3, 0);
  uint8_t color = get_param_int(argc, params, 4, 1);
  uint8_t filled = get_param_int(argc, params, 5, 0);

  if(filled)
    fbdraw.RectFill(x1, y1, x2 - x1, y2 - y1, color);
  else
    fbdraw.Rect(x1, y1, x2 - x1, y2 - y1, color);

  return(1);
}

int CMD_Circle(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t x = get_param_int(argc, params, 0, 0);
  uint16_t y = get_param_int(argc, params, 1, 0);
  uint16_t r = get_param_int(argc, params, 2, 0);
  uint8_t color = get_param_int(argc, params, 3, 1);
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
  uint8_t color = get_param_int(argc, params, 5, 1);
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
  uint8_t color = get_param_int(argc, params, 6, 1);
  uint8_t filled = get_param_int(argc, params, 7, 0);

  if(filled)
    fbdraw.TriangleFill(x1, y1, x2, y2, x3, y3, color);
  else
    fbdraw.Triangle(x1, y1, x2, y2, x3, y3, color);

  return(1);
}

int CMD_Print(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, NULL);
  char *text1 = new char[strlen(text) + 2];
  strcpy(text1,text);
  strcat(text1,"\n");
  uint8_t color = get_param_int(argc, params, 1, 1);

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

  if(SizeX > SSD1306_LCDWIDTH || SizeY > SSD1306_LCDHEIGHT)
  {
    v_setstr(retval, "SSD1306: BitBlt requires an 2D-array with maximal dimensions of the OLED.");
    return 0;
  }

  int32_t PosX = get_param_int(argc, params, 1, 0);  //Get pos x
  int32_t PosY = get_param_int(argc, params, 2, 0);  //Get pos y

  uint16_t DrawSizeX = SizeX;
  uint16_t DrawSizeY = SizeY;

  if(SSD1306_LCDWIDTH - PosX < SizeX) DrawSizeX = SSD1306_LCDWIDTH - PosX;
  if(SSD1306_LCDHEIGHT - PosY < SizeY) DrawSizeY = SSD1306_LCDHEIGHT - PosY;

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
            Framebuffer[xx + PosX + (yy+PosY)*SSD1306_LCDWIDTH] = 1;
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
            Framebuffer[xx + PosX + (yy+PosY)*SSD1306_LCDWIDTH] = 0;
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
            Framebuffer[xx + PosX + (yy+PosY)*SSD1306_LCDWIDTH] = 0;
          else if(get_array_elem_num(array, xx * SizeY + yy) == 0xFF) // White
            Framebuffer[xx + PosX + (yy+PosY)*SSD1306_LCDWIDTH] = 1;
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
  uint16_t w = get_param_int(argc, params, 2, SSD1306_LCDWIDTH - 1);
  uint16_t h = get_param_int(argc, params, 3, SSD1306_LCDHEIGHT - 1);

  if(x >= SSD1306_LCDWIDTH || y >= SSD1306_LCDHEIGHT) return(1);
  if(x + w > SSD1306_LCDWIDTH) w = SSD1306_LCDWIDTH - x;
  if(y + h > SSD1306_LCDHEIGHT) h = SSD1306_LCDHEIGHT - y;

  v_tomatrix(retval, w, h);

  uint32_t ii = 0;
  for(int16_t xx = x; xx < x + w; xx++)
  {
    for(int16_t yy = y; yy < y + h; yy++)
    {
      v_setreal(v_elem(retval, ii),  255*Framebuffer[xx + yy*SSD1306_LCDWIDTH]);  
      ii++;
    }
  }
  return(1);
}

void CopyFramebuffer(uint8_t *FB_8bit, uint8_t *FB_1bit)
{
  uint16_t ii = 0;

  for(uint8_t yy = 0; yy <= SSD1306_LCDHEIGHT - 8; yy += 8)
  {
    for(uint8_t xx = 0; xx < SSD1306_LCDWIDTH; xx++)
    {
      FB_1bit[ii] = (FB_8bit[xx + (yy + 7) * SSD1306_LCDWIDTH] << 7) |
                    (FB_8bit[xx + (yy + 6) * SSD1306_LCDWIDTH] << 6) |
                    (FB_8bit[xx + (yy + 5) * SSD1306_LCDWIDTH] << 5) |
                    (FB_8bit[xx + (yy + 4) * SSD1306_LCDWIDTH] << 4) |
                    (FB_8bit[xx + (yy + 3) * SSD1306_LCDWIDTH] << 3) |
                    (FB_8bit[xx + (yy + 2) * SSD1306_LCDWIDTH] << 2) |
                    (FB_8bit[xx + (yy + 1) * SSD1306_LCDWIDTH] << 1) |
                     FB_8bit[xx +  yy      * SSD1306_LCDWIDTH];
      ii++;
    }
  }
}


