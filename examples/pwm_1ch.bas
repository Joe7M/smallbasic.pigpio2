' SmallBASIC PiGPIO v2 - Single channel pwm
' 
' SDL version	 :  sudo sbasicg -m/usr/local/lib/smallbasic/ -n pwm_1ch.bas
' Console version: sudo sbasic -m /usr/local/lib/smallbasic/ pwm_1ch.bas

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
