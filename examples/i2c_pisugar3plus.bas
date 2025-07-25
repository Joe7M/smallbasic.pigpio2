' ---------------SmallBASIC PiGPIO v2------------------
'
' Generic I2C: PiSugar 3 Plus
' =====================================================
'
' This example demonstrates how control a PiSugar 3
' Plus using the generic I2C (SMBus) interface
'
' --------------         ----------
'  RPi          |       | PiSugar 3 Plus
'  PIN 3 (SDA)  |-------| SDA
'  PIN 5 (SCL)  |-------| SCL
'  PIN 6 (GND)  |-------| GND
'---------------         ---------
'
' To run type:
' sbasic -m /usr/local/lib/smallbasic/ i2c_pisugar3plus.bas
' sbasicg -m/usr/local/lib/smallbasic/ -r i2c_pisugar3plus.bas


import i2c

Print "Connect to PiSugar 3 Plus"
PiSugar = i2c.Open(0x57, "/dev/i2c-1")
Print "Connection established"

' Battery charge in percent
ChargeStatus = i2c.SmbusReadByte(PiSugar, 0x2A)
print ChargeStatus; "%"

' Voltage
VoltageHigh = i2c.SmbusReadByte(PiSugar, 0x22)
VoltageLow = i2c.SmbusReadByte(PiSugar, 0x23)
Voltage = (VoltageHigh lshift 8) + VoltageLow
print Voltage; "mV"

' Status
IsPowerConnected = i2c.SmbusReadByte(PiSugar, 0x02) rshift 7
print "USB Power connected: "; IsPowerConnected

' Read seconds from real time clock
print "Seconds: "; i2c.SmbusReadByte(PiSugar, 0x37)
