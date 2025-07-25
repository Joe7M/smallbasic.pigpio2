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
'  RP             |       |TFT
'  PIN 19 (MOSI)  |-------|DIN (MOSI) 
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
    return ((r BAND 0b11111000) BOR (g rshift 5) BOR ((g BAND 0b00011100) lshift 11) BOR ((b BAND 0b11111000) lshift 5)) 
end

const WHITE   = RGBto565(255,255,255)
const DRAW_FILLED = 1

const LCD_WIDTH = 240
const LCD_HEIGHT = 240
const SPI_DEVICE = "/dev/spidev0.0"
const GPIO_CHIP = "gpiochip0"
const PIN_RST = 27
const PIN_DC = 17
const PIN_BL = 22
const SPI_SPEED = 50000000  ' Test the max speed i.e. 1000000 (1MHz), 10000000 (10MHz), 50000000 (50MHz), 125000000 (125MHz)
const SPI_PAGESIZE = 4096

st7789.Open(LCD_WIDTH, LCD_HEIGHT, SPI_DEVICE, GPIO_CHIP, PIN_RST, PIN_DC, PIN_BL, SPI_SPEED, SPI_PAGESIZE)
st7789.Color(WHITE, 0)

TimeStart = ticks()
for yy = 20 to 220
  ii++
  st7789.Cls()
  st7789.Circle(119, yy,20, WHITE, DRAW_FILLED)
  st7789.Display()
next
print ii / (ticks() - TimeStart) * 1000; " FPS"

st7789.Close()

print("Done")
