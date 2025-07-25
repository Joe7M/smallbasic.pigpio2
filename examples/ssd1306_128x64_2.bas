' ---------------SmallBASIC PiGPIO v2------------------
'
' SSD1306 - 128x64 Pixel OLED Display
' =====================================================
'
' This example demonstrates how control a SSD1306 with
' 128x64 pixel
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
' sbasic -m /usr/local/lib/smallbasic/ ssd1306_128x64_2.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r ssd1306_128x64_2.bas


import ssd1306

ssd1306.Open("/dev/i2c-1")
delay(1000)

ssd1306.SetBrightness(128)
ssd1306.Cls()

ssd1306.Pset(10,0)
ssd1306.Line(0,0,127,63)
ssd1306.RoundRect(58,26,40,20,5)
ssd1306.Circle(118,10,5,1,1) 'Filled with white
ssd1306.Triangle(118,30, 113,45, 123,45, 1, 1) 'Filled with white

ssd1306.At(0,46)
ssd1306.SetTextSize(16)
ssd1306.Print("SmallBASIC")
ssd1306.Display()

'Let's move a image of a smiley with transparency over the screen

'Create an image array with 16x16 Pixels
dim SmilyGuyArray(15,15)

for yy = 0 to 15
  for xx = 0 to 15
    read d
    SmilyGuyArray(xx,yy) = d
  next
next

dim Background

for xx = 1 to 127
  ' Save the part of the OLED display, where we want to draw the smiley.
  Background = ssd1306.GetArray(xx, 25, 16, 16)
  ' Copy the array to the display: 
  ' Mode = 3 -> Pixel value: 0=Black;
  ' 255=White; Every other number is
  ' rendered transparent
  ssd1306.SetArray(SmilyGuyArray, xx, 25, 3)
  ssd1306.Display()
  ' Copy the saved background back to the display. This will remove the
  ' smiley. In the next iteration of the loop, you have the original
  ' background image.
  ssd1306.SetArray(Background, xx, 25)
next

delay(2000)
ssd1306.Close()

print("Done")


'Smiley Guy
'0=Black; 255=White; Every other number is transparent
DATA   1,  1,  1,255,255,255,255,255,255,255,255,255,255,  1,  1,  1
DATA   1,  1,255,255,255,255,255,255,255,255,255,255,255,255,  1,  1
DATA   1,  1,255,255,255,255,255,255,255,255,255,255,255,255,  1,  1
DATA   1,  1,255,255,  0,  0,  0,  0,  0,  0,  0,  0,255,255,  1,  1
DATA   1,  1,255,  0,255,255,255,  0,  0,255,255,255,  0,255,  1,  1
DATA   1,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  ,0  ,255,1
DATA 255,255,  0,  0,255,255,  0,  0,  0,  0,255,255,  0,  0,255,255
DATA 255,255,  0,  0,255,255,  0,255,  0,  0,255,255,  0,  0,255,255
DATA 255,255,  0,  0,  0,  0,  0,255,255,  0,  0,  0,  0,  0,255,255
DATA   1,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  1
DATA   1,255,  0,  0,255,  0,  0,  0,  0,  0,  0,255,  0,  0,255,  1
DATA   1,  1,255,  0,  0,255,  0,  0,  0,  0,255,  0,  0,255,  1,  1
DATA   1,  1,255,  0,  0,  0,255,255,255,255,  0,  0,  0,255,  1,  1
DATA   1,  1,  1,255,  0,  0,  0,  0,  0,  0,  0,  0,255,  1,  1,  1
DATA   1,  1,  1,  1,255,255,255,255,255,255,255,255,  1,  1,  1,  1
DATA   1,  1,  1,  1,  1,255,255,255,255,255,255,  1,  1,  1,  1,  1


