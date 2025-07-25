' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - Emmit a Trigger Pulse
' =====================================================
'
' This example demonstrates how to emmit a trigger
' pulse on pin 40 (GPIO21)
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_trigger.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_trigger.bas


import gpio

gpio.Open()
gpio.SetOutput(21)

for ii = 1 to 10
  gpio.Trigger(21)
  delay(200)
next

print "done"

