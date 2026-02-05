' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - Button
' =====================================================
'
' This example demonstrates how to use a push button with
' internal pull down resistor
'
' ---------------         ----------
'  RPi           |       |Button
'  PIN 7 (GPIO4) |-------|Pin 1 
'  PIN 1 (3.3V)  |-------|Pin 2
'----------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_button_1_setbias.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_button_1_setbias.bas
'

const BIAS_PULL_UP   = 0
const BIAS_PULL_DOWN = 1
const BIAS_DISABLE   = 2

import gpio

gpio.Open()
gpio.SetInput(4, BIAS_PULL_DOWN)

while(1)
  print gpio.Read(4)
  delay(500)
wend
