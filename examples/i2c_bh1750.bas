' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic I2C: BH1750 - Ambient Light Sensor
' =====================================================
'
' This example demonstrates how to use a BH1750 ambient
' light sensor with the generic I2C interface.
'
' --------------         ----------
'  RPi          |       | BH1750
'  PIN 1 (3.3V) |-------| VCC
'  PIN 3 (SDA)  |-------| SDA 
'  PIN 5 (SCL)  |-------| SCL
'  PIN 6 (GND)  |-------| GND
'---------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ i2c_bh1750.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r i2c_bh1750.bas

import i2c

Print "Connect to BH1750"
sensor = i2c.Open(0x23, "/dev/i2c-1")
Print "Connection established"

' Power down
i2c.write(sensor, 0x00)
' Power on
i2c.write(sensor, 0x01)
delay(500)

' Send "Continuously H-resolution mode" instruction
i2c.write(sensor, 0b00010000)
delay(200)

for i = 1 to 10
  d = i2c.Read(sensor, 2)
  ValueHighRes = ((d[0] lshift 8) BOR d[1]) / 1.2
  print "High resolution: " + valueHighRes + " lx"
  delay(500)
next