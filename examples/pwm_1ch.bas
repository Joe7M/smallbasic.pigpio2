' ---------------SmallBASIC PiGPIO v2------------------
'
' PWM - Pulse Width Modulation: Single Channel
' =====================================================
'
' This example demonstrates how to use single channel
' PWM
'
' Connect a LED with a 1KOhm resitor to pin 12 (PWM0)
' and pin 6 (GND)

' To run type:
' sbasic -m /usr/local/lib/smallbasic/ pwm_1ch.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r pwm_1ch.bas


import pwm

' Initialize 
pwm.open("pwmchip0")
pwm.Period(1000000)         ' in ns -> 1kHz
pwm.DutyCycle(0)            ' Active time in ns  -> 0 = off
pwm.Enable()                ' enable pwm0

' Fade from dark to bright
for ii = 1 to 1000000 Step 10000
  pwm.DutyCycle(ii)
  delay(10)
next
