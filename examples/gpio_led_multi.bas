' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic GPIO - LED
' =====================================================
'
' This example demonstrates how to turn on/off multiple
' LEDs using different methods and compare switching times.
'
' if you are just interested in the timing, you don't need
' to connect LEDs. Just run run the program without LEDs.

' To run type:
' sbasic -m /usr/local/lib/smallbasic/ gpio_led_multi.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r gpio_led_multi.bas

import gpio

gpio.Open()
gpio.SetOutput(18)
gpio.SetOutput(15)
gpio.SetOutput(14)
gpio.SetOutput(13)
gpio.SetOutput(12)
gpio.SetOutput(11)
gpio.SetOutput(10)
gpio.SetOutput(9)

starttime = ticks()
for ii = 1 to 100000
  v = !v
  gpio.Write(18, v)
  gpio.Write(15, v)
  gpio.Write(14, v)
  gpio.Write(13, v)
  gpio.Write(12, v)
  gpio.Write(11, v)
  gpio.Write(10, v)
  gpio.Write(9, v)
next
print "8 pin sequencial WRITE                "; round(100000 / ((ticks() - starttime)/1000) /1000); "kHz"

starttime = ticks()
for ii = 1 to 100000
  v = !v
  gpio.Write([18, 15, 14, 13, 12, 11, 10, 9], [v, v, v, v, v, v, v, v])
next
print "8 pin parallel WRITE(Array, Array)    "; round(100000 / ((ticks() - starttime)/1000) /1000); "kHz"  

starttime = ticks()
v = 0b10101010
for ii = 1 to 100000
  v = v XOR 0b11111111
  gpio.Write([18, 15, 14, 13, 12, 11, 10, 9], v)
next
print "8 pin parallel WRITE(Array, Number)   "; round(100000 / ((ticks() - starttime)/1000) /1000); "kHz"  

starttime = ticks()
for ii = 1 to 100000
  v = !v
  gpio.Write(18, v)
next
print "Single pin WRITE                      "; round(100000 / ((ticks() - starttime)/1000) /1000); "kHz"  

print "done"
