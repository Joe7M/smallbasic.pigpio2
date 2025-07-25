import st7789

func RGBto565(r,g,b)
    'return (((r BAND 0xF8) lshift 8) BOR ((g BAND 0xFC) lshift 3) BOR (b rshift 3))
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

'st7789.Open()
const LCD_WIDTH = 240
const LCD_HEIGHT = 240
const SPI_DEVICE = "/dev/spidev0.0"
const GPIO_CHIP = "gpiochip0"
const PIN_RST = 27
const PIN_DC = 17
const PIN_BL = 22
const SPI_SPEED = 50000000
const SPI_PAGESIZE = 4096


st7789.Open(LCD_WIDTH, LCD_HEIGHT, SPI_DEVICE, GPIO_CHIP, PIN_RST, PIN_DC, PIN_BL, SPI_SPEED, SPI_PAGESIZE)
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
