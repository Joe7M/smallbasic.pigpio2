' MPU6050 - Accelerometer, gyroscope and temperature sensor
' =========================================================
'
' This examample demonstrates how to use
' the MPU-6050 I2C sensor.
' 
' Connect the sensor to the IOIO-OTG board:
'
' ------         ------
'  IOIO |       |MPU6050
'  PIN 4|-------|SDA 
'  PIN 5|-------|SCL
'  GND  |-------|GND
'  3.3V |-------|VIN
'       |       |XCL
'       |       |XDA
'       |       |ADD
'       |       |INT
'-------         ------


import bmp180

bmp180.open("/dev/i2c-1", 0x77)

delay(500)

for ii = 1 to 50
    M = bmp180.StartMeasurement()
    
    locate 5,0
    print "T: "; M[0]; "  P: "; M[1]    

    delay(500)
    showpage
next