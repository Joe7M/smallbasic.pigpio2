' BMP180 - Measure temperature and pressure
' =========================================
'
' This examample demonstrates how to use
' the BMP180 I2C sensor.
'
' -------         -------
' BMP180 |       | RPi
'    VIN |-------| 3.3V
'    GND |-------| GND
'    SDA |-------| GPIO 2 (SDA)
'    SCL |-------| GPIO 3 (SCL)


import bmp180

bmp180.open("/dev/i2c-1", 0x77)

delay(500)

for ii = 1 to 50
  M = bmp180.StartMeasurement()

  locate 5,0
  print "T: "; M[0]; "  P: "; M[1]

  delay(500)
next