' SmallBASIC
'
' Example on how to use a SSD1306 OLED Display
' Tested with SmallBASIC 12.27
' By Joerg Siebenmorgen
' MIT Licence, 2024
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin ssd1306_2.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -n ssd1306_2.bas
'

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
