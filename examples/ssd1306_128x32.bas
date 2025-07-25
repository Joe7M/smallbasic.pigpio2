' ---------------SmallBASIC PiGPIO v2------------------
'
' SSD1306 - 128x32 Pixel OLED Display
' =====================================================
'
' This example demonstrates how control a SSD1306 with
' 128x32 pixel
'
' --------------         ----------
'  RPi          |       | SSD1306
'  PIN 1 (3.3V) |-------| VCC
'  PIN 3 (SDA)  |-------| SDA
'  PIN 5 (SCL)  |-------| SCL
'  PIN 6 (GND)  |-------| GND
' --------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ ssd1306_128x32.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r ssd1306_128x32.bas


import ssd1306

ssd1306.Open("/dev/i2c-1", 0x3C, 128, 32)
delay(1000)

ssd1306.SetBrightness(128)
ssd1306.Cls()

ssd1306.Pset(10,0)
ssd1306.Line(0,0,127,31)
ssd1306.RoundRect(58,5,40,20,5)
ssd1306.Circle(118,10,5,1,1) 'Filled with white

'ssd1306.At(0,46)
'ssd1306.SetTextSize(16)
'ssd1306.Print("SmallBASIC")
ssd1306.Display()

delay(2000)
ssd1306.Close()

print("Done")
