' Example on how to a push button
' Tested with SmallBASIC 12.28
' By Joerg Siebenmorgen
' MIT Licence, 2024
'
' Console version:  sbasic -m /home/pi/SmallBasicPIGPIO/bin ads1x15.bas
' SDL version:      sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r ads1x15.bas
'

import gpio

v = 0

gpio.Open()
gpio.SetInput(21)

while(1)
  print gpio.Read(21)
  delay(500)
wend

print "done"

