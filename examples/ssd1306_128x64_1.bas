import ssd1306

const ADDRESS = 0x3C
const WIDTH   = 128
const HEIGHT  = 64

Print "Connect to SSD1306 OLED display"
ssd1306.Open("/dev/i2c-1", ADDRESS)
Print "Connection established"

delay(2000)
ssd1306.Cls()
ssd1306.Circle(25, 40, 16, 1, true)
ssd1306.At(50,5): ssd1306.Print("I2C with", 1)
ssd1306.At(50,14): ssd1306.Print("SMALLBASIC", 1)
ssd1306.line(0, 0, 127, 63)
ssd1306.rect(0,0,127,63)

' Transfer framebuffer to display
s = ticks
ssd1306.Display()
print ticks - s
delay(2000)

' Set brightness
ssd1306.SetBrightness(20)
delay(1000)
ssd1306.SetBrightness(255)
delay(1000)

' Display on/off
ssd1306.DisplayOff()
delay(1000)
ssd1306.DisplayOn()
delay(1000)

' Set inverse
ssd1306.InvertDisplay()
delay(1000)
ssd1306.InvertDisplay()

print "done"