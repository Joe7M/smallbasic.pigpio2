' Example on how to a push button
' Tested with SmallBASIC 12.28
' By Joerg Siebenmorgen
' MIT Licence, 2024
'
' Console version:  sbasic -m /home/pi/SmallBasicPIGPIO/bin gpio_led.bas
' SDL version:      sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r gpio_led.bas
'

import gpio

gpio.Open()
gpio.SetOutput(18)

for ii = 1 to 10
  v = !v
  gpio.Write(18, v)
  delay(100)
next

print "done"

