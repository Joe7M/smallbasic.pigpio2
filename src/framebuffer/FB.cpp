//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas



#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <sys/mman.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "FB.hpp"

int fb_ActivePage = 0;
int MouseID;
int kbfd; // keyboard file for hidding cursor

std::vector<LinuxFrameBufferDraw> fblist;

LinuxFrameBufferDraw fbdraw;

int CMD_Init(int argc, slib_par_t *params, var_t *retval)
{
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  uint32_t screensize = 0;
  uint8_t *fbp = 0;
  LinuxFrameBufferDraw fbdrawtemp;
  uint8_t fb_device_id = 0;
  uint8_t UseDoubleBuffering = false;

  const char *fbdevice = get_param_str(argc, params, 0, "/dev/fb0");
  uint8_t UseMouse = get_param_int(argc, params, 1, 1);
  uint8_t HideText = get_param_int(argc, params, 2, 1);

  // Open the framebuffer device file for reading and writing
  fbfd = open(fbdevice, O_RDWR);
  if(fbfd == -1)
  {
    v_setstr(retval, "Cannot open framebuffer device.");
    return(0);
  }

  // Get variable screen information
  if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
  {
    v_setstr(retval, "Error reading variable screen info.");
    return(0);
  }

  // Change variable info
  //vinfo.bits_per_pixel = 32;
  //vinfo.xres = SizeX;
  //vinfo.yres = SizeY;
  //vinfo.xres_virtual = SizeX;

  // for double buffering double the physical size in y
  vinfo.xres_virtual = vinfo.xres;
  vinfo.yres_virtual = 2 * vinfo.yres; 
  if(!ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
  {
    // try to pan display for double buffer test
    vinfo.yoffset = vinfo.yres;
    vinfo.activate = FB_ACTIVATE_VBL;
    if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo))
    {
      printf("Double buffer not supported.\n");
      UseDoubleBuffering = false;
    }
    else
    {
      printf("Double buffer supported.\n");
      UseDoubleBuffering = true;
    }
    vinfo.yoffset = 0;
    ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo);

    //v_setstr(retval, "Error setting screen resolution and bit depth.");
    //return(0);
  }

  // Get new variable screen information
  if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
  {
    v_setstr(retval, "Error reading variable screen info.");
    return(0);
  }

  printf("Framebuffer initalized\nSize   : %d x %d\nBPP    : %d\nVirtual: %d x %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, vinfo.xres_virtual, vinfo.yres_virtual);

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
  {
    v_setstr(retval, "Error reading fixed screen info.");
    return(0);
  }

  // map framebuffer to user memory 
  //screensize = finfo.smem_len;
  screensize = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8 ;
  printf("Memory:  %d bytes\n", screensize);
  //printf("%d\n", finfo.line_length);

  fbp = (uint8_t*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if (fbp == MAP_FAILED)
  {
    v_setstr(retval, "Failed to map framebuffer memory.");
    return(0);
  }

  // Set Gray palette
  /*
  unsigned short r[256];
  unsigned short g[256];
  unsigned short b[256];

  for(ii = 0; ii < 256; ii++)
  {
      // note that Linux provides more precision (0-65535),
      // so we multiply ours (0-255) by 256
      r[ii] = ii << 8;
      g[ii] = ii << 8;
      b[ii] = ii << 8;
  }

  struct fb_cmap pal;
  pal.start = 0; // start our colors after the default 16
  pal.len = 256; // kludge to force bcm fb drv to commit palette...
  pal.red = r;
  pal.green = g;
  pal.blue = b;
  pal.transp = 0; // we want all colors non-transparent == null

  if (ioctl(fbfd, FBIOPUTCMAP, &pal))
  {
      printf("Error setting palette.\n");
      return(1);
  }
  */

  
  if(UseMouse)
  {
    // Open Mouse
    const char *pDevice = "/dev/input/mice";

    MouseID = open(pDevice, O_RDWR);
    if(MouseID == -1)
    {
      v_setstr(retval, "Cannot open mouse device.");
      return(0);
    }
  }

  if(HideText)
  {
    // hide cursor
    const char *kbfds = "/dev/tty0";
    kbfd = open(kbfds, O_WRONLY);
    if (kbfd >= 0)
    {
        ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
    }
    else
    {
      printf("Failed hidding cursor and text. Start with sudo might help.\n");
      //v_setstr(retval, "Failed hidding cursor.");
      //return(0);
    }
  }
  
  fbdrawtemp.Init(vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, fbp);
  fbdrawtemp.SetDoubleBuffering(UseDoubleBuffering);
  fbdrawtemp.SetFrameBufferFileID(fbfd);
  fbdrawtemp.SetVinfo(vinfo);

  fblist.push_back(fbdrawtemp);
  fb_device_id = fblist.size() - 1;

  v_setint(retval, fb_device_id);
  return(1);
}

int CMD_GetInfo(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);

  v_toarray1(retval, 4);
  v_setint(v_elem(retval, 0), fblist.at(id).GetWidth());
  v_setint(v_elem(retval, 1), fblist.at(id).GetHeight());
  v_setint(v_elem(retval, 2), fblist.at(id).GetBpp());
  v_setint(v_elem(retval, 3), fblist.at(id).GetDoubleBuffering());

  return(1);
}

int CMD_SwapBuffer(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);

  fblist.at(id).SwapBuffers();
  return(1);
}



int CMD_WaitForVSync(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint32_t dummy = 0;

  ioctl(fblist.at(id).GetFrameBufferFileID(), FBIO_WAITFORVSYNC, &dummy);

  return(1);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);

  munmap(fblist.at(id).GetFrameBufferAddress(), fbdraw.GetScreensize());

  ioctl(kbfd, KDSETMODE, KD_TEXT);
  close(kbfd);
  close(fblist.at(id).GetFrameBufferFileID());

  return(1);
}

int CMD_SetPixel(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x = get_param_int(argc, params, 1, 0);
  uint16_t y = get_param_int(argc, params, 2, 0);
  uint32_t c = get_param_int(argc, params, 3, fblist.at(id).GetFGColor());

  fblist.at(id).Pixel(x, y, c);
  return(1);
}

int CMD_Clear(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint32_t color = get_param_int(argc, params, 1, fbdraw.GetBGColor());

  fblist.at(id).Clear(color);
  return(1);
}

int CMD_Color(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  fblist.at(id).SetFGColor(get_param_int(argc, params, 1, 0));
  fblist.at(id).SetBGColor(get_param_int(argc, params, 2, fblist.at(id).GetBGColor()));

  return(1);
}

int CMD_Line(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x1 = get_param_int(argc, params, 1, 0);
  uint16_t y1 = get_param_int(argc, params, 2, 0);
  uint16_t x2 = get_param_int(argc, params, 3, 0);
  uint16_t y2 = get_param_int(argc, params, 4, 0);
  uint32_t c  = get_param_int(argc, params, 5, fblist.at(id).GetFGColor());

  fblist.at(id).Line(x1, y1, x2, y2, c);

  return(1);
}

int CMD_Rect(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x1 = get_param_int(argc, params, 1, 0);
  uint16_t y1 = get_param_int(argc, params, 2, 0);
  uint16_t x2 = get_param_int(argc, params, 3, 0);
  uint16_t y2 = get_param_int(argc, params, 4, 0);
  uint32_t c  = get_param_int(argc, params, 5, fblist.at(id).GetFGColor());
  uint8_t filled = get_param_int(argc, params, 6, 0);

  if(filled)
    fblist.at(id).RectFill(x1, y1, x2, y2, c);
  else
    fblist.at(id).Rect(x1, y1, x2, y2, c);

  return(1);
}

int CMD_Circle(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x = get_param_int(argc, params, 1, 0);
  uint16_t y = get_param_int(argc, params, 2, 0);
  uint16_t r = get_param_int(argc, params, 3, 0);
  uint32_t c = get_param_int(argc, params, 4, fblist.at(id).GetFGColor());
  uint8_t filled = get_param_int(argc, params, 5, 0);

  if(filled)
    fblist.at(id).CircleFill(x, y, r, c);
  else
    fblist.at(id).Circle(x, y, r, c);

  return(1);
}

int CMD_RoundRect(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x1 = get_param_int(argc, params, 1, 0);
  uint16_t y1 = get_param_int(argc, params, 2, 0);
  uint16_t x2 = get_param_int(argc, params, 3, 0);
  uint16_t y2 = get_param_int(argc, params, 4, 0);
  uint8_t r = get_param_int(argc, params, 5, 4);
  uint32_t c = get_param_int(argc, params, 6, fblist.at(id).GetFGColor());
  uint8_t filled = get_param_int(argc, params, 7, 0);

  if(filled)
    fblist.at(id).RoundRectFill(x1, y1, x2, y2, r, c);
  else
    fblist.at(id).RoundRect(x1, y1, x2, y2, r, c);
  return(1);
}


int CMD_Triangle(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x1 = get_param_int(argc, params, 1, 0);
  uint16_t y1 = get_param_int(argc, params, 2, 0);
  uint16_t x2 = get_param_int(argc, params, 3, 0);
  uint16_t y2 = get_param_int(argc, params, 4, 0);
  uint16_t x3 = get_param_int(argc, params, 5, 0);
  uint16_t y3 = get_param_int(argc, params, 6, 0);
  uint32_t c = get_param_int(argc, params, 7, fblist.at(id).GetFGColor());
  uint8_t filled = get_param_int(argc, params, 8, 0);

  if(filled)
    fblist.at(id).TriangleFill(x1, y1, x2, y2, x3, y3, c);
  else
    fblist.at(id).Triangle(x1, y1, x2, y2, x3, y3, c);

  return(1);
}

int CMD_Print(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  const char *text = get_param_str(argc, params, 1, NULL);
  char *text1 = new char[strlen(text) + 2];
  strcpy(text1,text);
  strcat(text1,"\n");

  uint32_t color = get_param_int(argc, params, 2, fblist.at(id).GetFGColor());

  fblist.at(id).Text(text1, color);

  delete[] text1;

  return(1);
}

int CMD_SetTextSize(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint8_t TextSize = get_param_int(argc, params, 1, 8);

  fblist.at(id).SetTextSize(TextSize);

  return(1);
}

int CMD_At(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t x = get_param_int(argc, params, 1, 0);
  uint16_t y = get_param_int(argc, params, 2, 0);

  fblist.at(id).GotoXY(x, y);

  return(1);
}

int CMD_CopyToDisplay(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t xres = fblist.at(id).GetWidth();
  uint16_t yres = fblist.at(id).GetHeight();

  if(!is_param_array(argc, params, 1))
  {
    v_setstr(retval, "FB: BitBlt requires an array");
    return 0;
  }

  var_p_t array = params[1].var_p;  //Get array

  if(v_maxdim(array) != 2)
  {
    v_setstr(retval, "FB: BitBlt requires an 2D-array");
    return 0;
  }

  uint16_t SizeX = v_ubound(array, 0) - v_lbound(array, 0) + 1;
  uint16_t SizeY = v_ubound(array, 1) - v_lbound(array, 1) + 1;

  if(SizeX > xres || SizeY > yres)
  {
    v_setstr(retval, "FB: BitBlt requires an 2D-array with maximal dimensions of the Framebuffer.");
    return 0;
  }

  uint16_t PosX = get_param_int(argc, params, 2, 0);  //Get pos x
  uint16_t PosY = get_param_int(argc, params, 3, 0);  //Get pos y

  if(PosX >= xres || PosY >= yres) return(1);

  uint16_t DrawSizeX = SizeX;
  uint16_t DrawSizeY = SizeY;

  if(xres - PosX < SizeX) DrawSizeX = xres - PosX;
  if(yres - PosY < SizeY) DrawSizeY = yres - PosY;

  uint8_t BitBltMode = get_param_int(argc, params, 4, 0);  //Get mode

  uint32_t color;
  switch(BitBltMode)
  {
    case 1: // if color is 0 then transparent
      for(uint16_t xx = 0; xx < DrawSizeX; xx++)
      {
        for(uint16_t yy = 0; yy < DrawSizeY; yy++)
        {
          color = get_array_elem_num(array, xx * SizeY + yy);
          if(color > 0) // not transparent
            fblist.at(id).Pixel(PosX + xx, PosY + yy, color);
        }
      }
      break;
    default: //No transparancy
      for(uint16_t xx = 0; xx < DrawSizeX; xx++)
      {
        for(uint16_t yy = 0; yy < DrawSizeY; yy++)
        {
          color = get_array_elem_num(array, xx * SizeY + yy);
          fblist.at(id).Pixel(PosX + xx, PosY + yy, color);
        }
      }
  }
  return(1);
}

int CMD_CopyFromDisplay(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint16_t xres = fblist.at(id).GetWidth();
  uint16_t yres = fblist.at(id).GetHeight();

  uint16_t x = get_param_int(argc, params, 1, 0);
  uint16_t y = get_param_int(argc, params, 2, 0);
  uint16_t w = get_param_int(argc, params, 3, xres - 1);
  uint16_t h = get_param_int(argc, params, 4, yres - 1);

  v_tomatrix(retval, w, h);

  if(x >= xres || y >= yres) return(1);
  if(x + w > xres) w = xres - x;
  if(y + h > yres) h = yres - y;

  uint32_t ii = 0;
  for(uint16_t xx = x; xx < x + w; xx++)
  {
    for(uint16_t yy = y; yy < y + h; yy++)
    {
      v_setint(v_elem(retval, ii), fblist.at(id).GetPixel(xx,yy));
      ii++;
    }
  }
  return(1);
}

int CMD_GetMouse(int argc, slib_par_t *params, var_t *retval)
{
  int bytes;
  unsigned char data[3];
  int left = -1, middle = -1, right = -1;
  signed char x = -1, y = -1;

  // Read Mouse
  bytes = read(MouseID, data, sizeof(data));

  if(bytes > 0)
  {
    left = data[0] & 0x1;
    right = data[0] & 0x2;
    middle = data[0] & 0x4;
    x = data[1];
    y = data[2];
  }

  v_toarray1(retval, 5);
  v_setint(v_elem(retval, 0), x);
  v_setint(v_elem(retval, 1), y);
  v_setint(v_elem(retval, 2), left);
  v_setint(v_elem(retval, 3), middle);
  v_setint(v_elem(retval, 4), right);

  return(1);
}

void LinuxFrameBufferDraw::SwapBuffers(void)
{
  if(UseDoubleBuffer)
  {
    Offset =  CurrentDoubleBufferPage * height * width;
    SetOffset(Offset);

    CurrentDoubleBufferPage = 1 - CurrentDoubleBufferPage;

    vinfo.yoffset = CurrentDoubleBufferPage * height;
    vinfo.activate = FB_ACTIVATE_VBL;

    if(ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo))
      printf("Error pan display\n");
  }
}
