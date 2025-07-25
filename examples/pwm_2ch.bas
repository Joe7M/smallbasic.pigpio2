' ---------------SmallBASIC PiGPIO v2------------------
'
' PWM - Pulse Width Modulation: Dual Channel
' =====================================================
'
' This example demonstrates how to use dual channel
' PWM
'
' Connect a LED with a 1KOhm resitor to pin 12 (PWM0)
' and pin 6 (GND).
' Connect a second LED with a 1KOhm resitor to pin 35
' (PWM1) and pin 39 (GND).

' To run type:
' sbasic -m /usr/local/lib/smallbasic/ pwm_2ch.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r pwm_2ch.bas

import pwm

' Initialize 
pwm.open("pwmchip0")
pwm.Period(1000000, 0)      ' pwm0: in ns -> 1kHz
pwm.Period(1000000, 1)      ' pwm1: in ns -> 1kHz
pwm.DutyCycle(0,0)          ' pwm0: Active time in ns  -> 0 = off
pwm.DutyCycle(0,1)          ' pwm1: Active time in ns  -> 0 = off
pwm.Enable(1,0)             ' enable pwm0
pwm.Enable(1,1)             ' enable pwm1

' Fade from dark to bright at pwm0
' and from bright to dark at pwm1

for ii = 0 to 1000000 Step 10000
  pwm.DutyCycle(ii,0)             ' pwm0  
  pwm.DutyCycle(1000000 - ii,1)   ' pwm1
  delay(10)
next
