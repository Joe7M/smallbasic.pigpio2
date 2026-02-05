' ---------------SmallBASIC PiGPIO v2------------------
'
' ST7789 - TFT RGB Display Controller
' =====================================================
'
' This example demonstrates how to drive a TFT display
' with a ST7789 controller.
'
' There exist many TFT displays using the ST7789
' controller. This examples is written for the
' Waveshare 1.3inch LCD module with 240x240 pixels.
' With some minor modification especially for the pins
' and pixels, TFTs from other manufacturers should also
' work. Be carefull with VCC. If you are using a bare
' TFT, then drive it only with 3.3V. Many breakout
' boards (i.e. Adafruit) support 5V.
'
' ----------------         ----------
'  RPi            |       |TFT
'  PIN 19 (MOSI)  |-------|DIN (MOSI, SDA)
'  PIN 23 (SCLK)  |-------|CLK (SCL)
'  PIN 24 (CE0)   |-------|CS
'  PIN 11 (GPIO17)|-------|DC
'  PIN 13 (GPIO27)|-------|RST
'  PIN 15 (GPIO22)|-------|BL
'  GND            |-------|GND
'  5V             |-------|VIN
' ----------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ st7789_2.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r st7789_2.bas


import st7789

func RGBto565(r,g,b)
    'Convert RGB 888 to 16bit RGB565 and swap bytes
    return ( (r BAND 0b11111000) BOR (g rshift 5) BOR ((g BAND 0b00011100) lshift 11) BOR ((b BAND 0b11111000) lshift 5) ) 
end

const BLACK   = 0
const BLUE    = RGBto565(0  ,  0,255)
const RED     = RGBto565(255,  0,0  )
const GREEN   = RGBto565(  0,255,0  )
const CYAN    = RGBto565(  0,255,255)
const MAGENTA = RGBto565(255,  0,255)
const YELLOW  = RGBto565(255,255,0  )
const WHITE   = RGBto565(255,255,255)
const DRAW_FILLED = 1

const LCD_WIDTH    = 240
const LCD_HEIGHT   = 240
const SPI_DEVICE   = "/dev/spidev0.0"
const GPIO_CHIP    = "/dev/gpiochip0"
const PIN_RST      = 27
const PIN_DC       = 17
const PIN_BL       = 22
const SPI_SPEED    = 50000000
const SPI_PAGESIZE = 4096

st7789.Open(LCD_WIDTH, LCD_HEIGHT, SPI_DEVICE, GPIO_CHIP, PIN_RST, PIN_DC, PIN_BL, SPI_SPEED, SPI_PAGESIZE)
' if you have a 240x240 TFT and don't need any extra configuration, then use:
' st7789.Open()
st7789.Cls()
st7789.Color(WHITE, 0)

st7789.Line(0,0,239,239)
st7789.RoundRect(60,60,120,120,5, BLUE)
st7789.Circle(119,119,50, RED, DRAW_FILLED)

st7789.At(30,10)
st7789.SetTextSize(24)
st7789.Print("SmallBASIC")

st7789.Rect(  0,209, 30,30,   BLACK, DRAW_FILLED)
st7789.Rect( 29,209, 30,30,     RED, DRAW_FILLED)
st7789.Rect( 59,209, 30,30,   GREEN, DRAW_FILLED)
st7789.Rect( 89,209, 30,30,    BLUE, DRAW_FILLED)
st7789.Rect(119,209, 30,30,    CYAN, DRAW_FILLED)
st7789.Rect(149,209, 30,30, MAGENTA, DRAW_FILLED)
st7789.Rect(179,209, 30,30,  YELLOW, DRAW_FILLED)
st7789.Rect(209,209, 30,30,   WHITE, DRAW_FILLED)

a = ticks()
st7789.Display()
print ticks() - a

st7789.Close()

print("Done")
