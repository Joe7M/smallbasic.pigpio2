' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - Emmit a Trigger Pulse
' =====================================================
'
' This example demonstrates how to emmit a trigger
' pulse.
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_trigger.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_trigger.bas


import gpio

gpio.Open()
gpio.SetOutput(18)  ' GPIO18 -> Pin 12

for ii = 1 to 10
  gpio.Trigger(18, 50, 1)
  delay(200)
next

print "done"

