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
#include "FBdraw.hpp"

#define FB_swap(a, b) { uint32_t t = a; a = b; b = t; }

const uint8_t font[] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46
};

const uint8_t font2[] = {
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};

FBDraw::FBDraw()
{
  TextSize = 1;
  PosX = 0;
  PosY = 0;
  TextWrap = true;
  BG_Color = 0x00000000;
  FG_Color = 0x00FFFFFF;
}

void FBDraw::Init(uint16_t w, uint16_t h, uint8_t b, uint8_t *fb)
{
  width = w;
  height = h;
  bpp = b;
  FrameBuffer = fb;
}

void FBDraw::Pixel(uint16_t x, uint16_t y, uint32_t c)
{
  if((x >= width) || (y >= height)) return;

  uint32_t pix_offset = x + y * width + Offset;

  switch(bpp)
  {
    case 8:
    {
      uint8_t* p = (uint8_t*)FrameBuffer;
      p[pix_offset] = (uint8_t)c;
    }
    break;
    case 16:
    {
      uint16_t* p = (uint16_t*)FrameBuffer;
      p[pix_offset] = (uint16_t)c;
    }
    break;
    case 32:
    {
      uint32_t* p = (uint32_t*)FrameBuffer;
      p[pix_offset] = c;
    }
    break;
  }
}

uint32_t FBDraw::GetPixel(uint16_t x, uint16_t y)
{
  if((x >= width) || (y >= height)) return 0;

  uint32_t pix_offset = x + y * width  + Offset;

  switch(bpp)
  {
    case 8:
    {
      uint8_t* p = (uint8_t*)FrameBuffer;
      return (uint32_t)(p[pix_offset]);
    }
    case 16:
    {
      uint16_t* p = (uint16_t*)FrameBuffer;
      return (uint32_t)(p[pix_offset]);
    }
    case 32:
    {
      uint32_t* p = (uint32_t*)FrameBuffer;
      return (uint32_t)(p[pix_offset]);
    }
  }
  return 0;
}

void FBDraw::Clear(uint32_t color)
{
  switch(bpp)
  {
    case 8:
    {
      uint8_t* p = (uint8_t*)FrameBuffer + Offset;
      uint32_t sz = width*height + Offset;
      while (sz--)
        *p++ = (uint8_t)color;
    }
    break;
    case 16:
    {
      uint16_t* p = (uint16_t*)FrameBuffer + Offset;
      uint32_t sz = width*height;
      while (sz--)
        *p++ = (uint16_t)color;
    }
    break;
    case 32:
    {
      uint32_t* p = (uint32_t*)FrameBuffer + Offset;
      uint32_t sz = width*height;
      while (sz--)
        *p++ = (uint32_t)color;
    }
    break;
  }
}

void FBDraw::Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
{
  uint16_t steep;
  int32_t ystep;
  int32_t dx, dy;
  int32_t err;

  steep = abs(y1 - y0) > abs(x1 - x0);

  if(steep)
  {
    FB_swap(x0, y0);
    FB_swap(x1, y1);
  }

  if(x0 > x1)
  {
    FB_swap(x0, x1);
    FB_swap(y0, y1);
  }

  dx = x1 - x0;
  dy = abs(y1 - y0);
  err = dx / 2;

  if(y0 < y1)
      ystep = 1;
  else
      ystep = -1;

  if(steep)
  {
    for(; x0 <= x1; x0++)
    {
      Pixel(y0, x0, color);
      err -= dy;
      if(err < 0)
      {
        y0  += ystep;
        err += dx;
      }
    }
  }
  else
  {
    for(; x0 <= x1; x0++)
    {
      Pixel(x0, y0, color);
      err -= dy;
      if(err < 0)
      {
        y0  += ystep;
        err += dx;
      }
    }
  }
}


void FBDraw::FastHLine(uint32_t x, uint32_t y, uint32_t w, uint32_t color)
{
  if(x >= width || y >= height) return;
  if(x + w > width) w = width - x;

  uint32_t pix_offset = x + y * width + Offset;

  switch(bpp)
  {
    case 8:
    {
      uint8_t* p = (uint8_t*)FrameBuffer + pix_offset;
      while (w--)
          *p++ = (uint8_t)color;
    }
    break;
    case 16:
    {
      uint16_t* p = (uint16_t*)FrameBuffer + pix_offset;
      while (w--)
          *p++ = (uint16_t)color;
    }
    break;
    case 32:
    {
      uint32_t* p = (uint32_t*)FrameBuffer + pix_offset;
      while (w--)
          *p++ = (uint32_t)color;
    }
    break;
  }
}

void FBDraw::FastVLine(uint32_t x, uint32_t y, uint32_t h, uint32_t color)
{
  if(x >= width || y >= height) return;
  if(y + h > height) h = height - y;

  uint32_t pix_offset = x + y * width + Offset;

  switch(bpp)
  {
    case 8:
    {
      uint8_t* p = (uint8_t*)FrameBuffer + pix_offset;
      while (h--)
      {
        *p = (uint8_t)color;
        p += width;
      }
    }
    break;
    case 16:
    {
      uint16_t* p = (uint16_t*)FrameBuffer + pix_offset;
      while (h--)
      {
        *p = (uint16_t)color;
        p += width;
      }
    }
    break;
    case 32:
    {
      uint32_t* p = (uint32_t*)FrameBuffer + pix_offset;
      while (h--)
      {
        *p = color;
        p += width;
      }
    }
    break;
  }
}

// Draw a rectangle
void FBDraw::Rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
  if(x2 < x1) FB_swap(x1, x2);
  if(y2 < y1) FB_swap(x2, x1);

  uint16_t w = x2 - x1;
  uint16_t h = y2 - y1;

  FastHLine(x1, y1, w, color);
  FastHLine(x1, y1 + h - 1, w, color);
  FastVLine(x1, y1, h, color);
  FastVLine(x1 + w - 1, y1, h, color);
}

void FBDraw::RectFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
  if(x2 < x1) FB_swap(x1, x2);
  if(y2 < y1) FB_swap(x2, x1);

  uint16_t w = x2 - x1;
  uint16_t h = y2 - y1;

  for (uint32_t i = y1; i < y1 + h; i++)
  {
    FastHLine(x1, i, w, color);
  }
}


void FBDraw::Circle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color)
{
  int32_t f = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * r;
  uint16_t x = 0;
  uint16_t y = r;

  Pixel(x0    , y0 + r, color);
  Pixel(x0    , y0 - r, color);
  Pixel(x0 + r, y0    , color);
  Pixel(x0 - r, y0    , color);

  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    Pixel(x0 + x, y0 + y, color);
    Pixel(x0 - x, y0 + y, color);
    Pixel(x0 + x, y0 - y, color);
    Pixel(x0 - x, y0 - y, color);
    Pixel(x0 + y, y0 + x, color);
    Pixel(x0 - y, y0 + x, color);
    Pixel(x0 + y, y0 - x, color);
    Pixel(x0 - y, y0 - x, color);
  }
}


void FBDraw::CircleHelper(uint32_t x0, uint32_t y0, uint32_t r, uint32_t cornername, uint32_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * r;
  uint32_t x    = 0;
  uint32_t y    = r;

  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y+= 2;
      f+= ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    switch(cornername)
    {
      case 4:
        Pixel(x0 + x, y0 + y, color);
        Pixel(x0 + y, y0 + x, color);
        break;
      case 2:
        Pixel(x0 + x, y0 - y, color);
        Pixel(x0 + y, y0 - x, color);
        break;
      case 8:
        Pixel(x0 - y, y0 + x, color);
        Pixel(x0 - x, y0 + y, color);
        break;
      case 1:
        Pixel(x0 - y, y0 - x, color);
        Pixel(x0 - x, y0 - y, color);
        break;
    }
  }

}

void FBDraw::CircleFill(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color)
{
  FastVLine(x0, y0 - r, 2 * r + 1, color);
  CircleFillHelper(x0, y0, r, 3, 0, color);
}

void FBDraw::CircleFillHelper(uint32_t x0, uint32_t y0, uint32_t r, uint32_t cornername, uint32_t delta, uint32_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * r;
  uint16_t x    = 0;
  uint16_t y    = r;
  int16_t px = x;
  int16_t py = y;

  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if(x < (y + 1))
    {
      if (cornername & 1) FastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      if (cornername & 2) FastVLine(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py)
    {
      if (cornername & 1) FastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      if (cornername & 2) FastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}


// Draw a rounded rectangle
void FBDraw::RoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint32_t color)
{
  if(x2 < x1) FB_swap(x1, x2);
  if(y2 < y1) FB_swap(x2, x1);

  uint16_t w = x2 - x1;
  uint16_t h = y2 - y1;

  if(x1 >= width || y1 >=height) return;
  if(2 * r > w) r = w/2;
  if(2 * r > h) r = h/2;

  FastHLine(x1 + r, y1, w - 2 * r, color); // Top
  FastHLine(x1 + r, y1 + h - 1, w - 2 * r, color); // Bottom
  FastVLine(x1, y1 + r, h - 2 * r, color); // Left
  FastVLine(x1 + w - 1, y1 + r, h - 2 * r, color); // Right
  // draw four corners
  CircleHelper(x1 + r, y1 + r, r, 1, color);
  CircleHelper(x1 + w - r - 1, y1 + r, r, 2, color);
  CircleHelper(x1 + w - r - 1, y1 + h - r - 1, r, 4, color);
  CircleHelper(x1 + r, y1 + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void FBDraw::RoundRectFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint32_t color )
{
  if(x2 < x1) FB_swap(x1, x2);
  if(y2 < y1) FB_swap(x2, x1);

  uint16_t w = x2 - x1;
  uint16_t h = y2 - y1;

  if(x1 >= width || y1 >=height) return;
  if(2 * r > w) r = w/2;
  if(2 * r > h) r = h/2;

  RectFill(x1 + r, y1, x2 - r, y2, color);
  // draw four corners
  CircleFillHelper(x1 + w - r - 1, y1 + r, r, 1, h - 2 * r - 1, color);
  CircleFillHelper(x1 + r        , y1 + r, r, 2, h - 2 * r - 1, color);
}


// Draw a triangle
void FBDraw::Triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
  Line(x0, y0, x1, y1, color);
  Line(x1, y1, x2, y2, color);
  Line(x2, y2, x0, y0, color);
}

// Fill a triangle
void FBDraw::TriangleFill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
  int32_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1)
  {
    FB_swap(y0, y1);
    FB_swap(x0, x1);
  }
  if (y1 > y2)
  {
    FB_swap(y2, y1);
    FB_swap(x2, x1);
  }
  if (y0 > y1)
  {
    FB_swap(y0, y1);
    FB_swap(x0, x1);
  }
  if(y0 == y2)
  { 
    a = b = x0;
    if(x1 < a)
      a = x1;
    else if(x1 > b)
      b = x1;
    if(x2 < a)
      a = x2;
    else if(x2 > b)
      b = x2;
    FastHLine(a, y0, b - a + 1, color);
    return;
  }

  int32_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1;
  int32_t  sa   = 0, sb   = 0;

  if(y1 == y2)
    last = y1;   // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for(y = y0; y <= last; y++)
  {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if(a > b) FB_swap(a, b);
    FastHLine(a, y, b - a + 1, color);
  }

  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y <= y2; y++)
  {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if(a > b) FB_swap(a, b);
    FastHLine(a, y, b - a + 1, color);
  }
}

void FBDraw::Text(char *_text, uint32_t color)
{
  while(*_text != '\0')
    Print(*_text++, color);
}

// move cursor to position (x, y)
void FBDraw::GotoXY(uint16_t x, uint16_t y)
{
  if((x >= width) || (y >= height)) return;
  PosX = x;
  PosY = y;
}

// set text size
void FBDraw::SetTextSize(uint8_t size)
{
  if(size < 8) size = 8;
  TextSize = size / 8;
}

// print single char
//    \a  Set cursor position to upper left (0, 0)
//    \b  Move back one position
//    \n  Go to start of current line
//    \r  Go to line below
//
void FBDraw::Print(uint8_t c, uint32_t color)
{
  int16_t i, j, line;

  if (c == ' ' && PosX == 0 && TextWrap) return;

  if(c == '\a')
  {
    PosX = PosY = 0;
    return;
  }

  if( (c == '\b') && (PosX >= TextSize * 6) )
  {
    PosX -= TextSize * 6;
    return;
  }

  if(c == '\r')
  {
    PosX = 0;
    return;
  }

  if(c == '\n')
  {
    PosX = 0;
    PosY += TextSize * 8;
    if((PosY + TextSize * 7) > height) PosY = 0;
    return;
  }

  if((c < ' ') || (c > '~')) c = '?';

  for(i = 0; i < 5; i++ )
  {
    if(c < 'S') 
      line = font[(c - ' ') * 5 + i];
    else
      line = font2[(c - 'S') * 5 + i];

    for(j = 0; j < 7; j++, line >>= 1)
    {
      if(line & 0x01)  //Transparent background
      {
        if(TextSize == 1)
        {
          Pixel(PosX + i, PosY + j, color);
        }
        else
        {
          RectFill(PosX + (i * TextSize), PosY + (j * TextSize), PosX + (i + 1) * TextSize, PosY + (j + 1) * TextSize, color);
        }
      }
    }
  }

  PosX += TextSize * 6;

  if(PosX > (width + TextSize * 6)) PosX = width;

  if (TextWrap && (PosX + (TextSize * 5)) > width)
  {
    PosX = 0;
    PosY += TextSize * 8;
    if((PosY + TextSize * 7) > height) PosY = 0;
  }
}

