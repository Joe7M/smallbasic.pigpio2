//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2024
//
//SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
//Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

#ifndef _FBDRAW_H_
#define _FBDRAW_H_

class FBDraw
{
  public:

    FBDraw();

    virtual void Pixel(uint16_t x, uint16_t y, uint32_t color);
    virtual uint32_t GetPixel(uint16_t x, uint16_t y);
    virtual void Clear(uint32_t color);

    void Init(uint16_t w, uint16_t h, uint8_t b, uint8_t *fb);
    void Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
    void Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
    void RectFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
    void Circle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color);
    void CircleFill(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color);
    void RoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint32_t color);
    void RoundRectFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint32_t color );
    void Triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
    void TriangleFill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
    void GotoXY(uint16_t x, uint16_t y);
    void SetTextSize(uint8_t t_size);
    void Text(char *_text, uint32_t color);
    uint16_t GetWidth(void) {return width;};
    uint16_t GetHeight(void) {return height;};
    uint8_t  GetBpp(void) {return bpp;};
    uint8_t* GetFrameBufferAddress(void) {return FrameBuffer;};
    uint32_t GetScreensize(void) {return width * height * bpp / 8;};
    uint32_t GetFGColor(void) {return FG_Color;};
    uint32_t GetBGColor(void) {return BG_Color;};
    void SetFGColor(uint32_t c) {FG_Color = c;};
    void SetBGColor(uint32_t c) {BG_Color = c;};
    void SetOffset(uint32_t Off) {Offset = Off;};

  protected:

    uint16_t width, height;
    uint8_t bpp;
    uint8_t *FrameBuffer;
    uint8_t TextSize;
    uint16_t PosX, PosY;
    uint8_t TextWrap;
    uint32_t BG_Color, FG_Color;
    uint32_t Offset = 0;

    void FastHLine(uint32_t x, uint32_t y, uint32_t w, uint32_t color);
    void FastVLine(uint32_t x, uint32_t y, uint32_t h, uint32_t color);
    void CircleHelper(uint32_t x0, uint32_t y0, uint32_t r, uint32_t cornername, uint32_t color);
    void CircleFillHelper(uint32_t x0, uint32_t y0, uint32_t r, uint32_t cornername, uint32_t delta, uint32_t color);
    void Print(uint8_t c, uint32_t color);

};


#endif /* !_FBDRAW_H_ */  
