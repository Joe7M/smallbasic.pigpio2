' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - LED
' =====================================================
'
' This example demonstrates how to turn on/off a LED.
'
' Connect a LED and a 1kOhm resistor to the RPi pin 7
' (GPIO4) and pin 6 (GND).

' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_led.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_led.bas

import gpio

gpio.Open()
gpio.SetOutput(18)

for ii = 1 to 10
  v = !v
  gpio.Write(18, v)
  delay(100)
next

print "done"
