' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - Wait for Trigger
' =====================================================
'
' This example demonstrates how to wait for a trigger
' pulse on pin 40 (GPIO21).
''
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_wait_for_trigger.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_wait_for_trigger.bas

import gpio

gpio.Open()
gpio.SetInput(21)

result = gpio.WaitTrigger(21, 5)

select case result
  case  0: print "Time out"
  case  1: print "Rising edge detected"
  case -1: print "Error"
end select

print "done"
