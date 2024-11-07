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

' If ADD is open or connected to GND, 0x68 as I2C address 
' will be used. Otherwise 0x69. 
' Don't connect XCL, XDA, ADD and INT
' 
' "https://github.com/tockn/MPU6050_tockn" was very helpful
' to get the sensor working

import mpu6050

const ADDRESS = 0x68

mpu6050.open("/dev/i2c-1", ADDRESS)

delay(500)

for ii = 1 to 1000
    A = mpu6050.Acceleration()
    G = mpu6050.Gyroscope()
    T = mpu6050.Temperature()

    locate 5,0
    print "Acc: [";
    print USING "##.00 "; A[0], A[1], A[2];
    print "]  Gryo: [";
    print USING "####.00 "; G[0], G[1], G[2];
    print "] Temp : ";
    print USING "##.00 "; T

    delay(100)
    showpage
next