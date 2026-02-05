' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - Button
' =====================================================
'
' This example demonstrates how to check two push buttons
' simultaneous.
'
' -----------------         ----------
'  RPi             |       |Button 1
'  PIN  7 (GPIO4)  |-------|Pin 1 
'  PIN  9 (GND)    |-------|Pin 2
'                  |       |Button 2
'  PIN 11 (GPIO17) |-------|Pin 1 
'  PIN 14 (GND)    |-------|Pin 2
'----------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_button_multi.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_button_multi.bas
'

import gpio

gpio.Open()
gpio.SetInput(4)
gpio.SetInput(17)

while(1)
  print gpio.Read([4,17])
  delay(500)
wend
