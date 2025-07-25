' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - Button
' =====================================================
'
' This example demonstrates how to use a push button.
'
' ---------------         ----------
'  RPi           |       |Button
'  PIN 7 (GPIO4) |-------|Pin 1 
'  PIN 2 (GND)   |-------|Pin 2
'----------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_button_1.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_button_1.bas
'

import gpio

gpio.Open()
gpio.SetInput(4)

while(1)
  print gpio.Read(4)
  delay(500)
wend
