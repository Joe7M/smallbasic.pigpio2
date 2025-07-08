' SmallBASIC PiGPIO v2 - Dual channel pwm
' 
' SDL version	 :  sudo sbasicg -m/usr/local/lib/smallbasic/ -n pwm_2ch.bas
' Console version: sudo sbasic -m /usr/local/lib/smallbasic/ pwm_2ch.bas

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
