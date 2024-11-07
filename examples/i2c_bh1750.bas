import i2c

Print "Connect to BH1750"
sensor = i2c.Open(0x23, "/dev/i2c-1")
Print "Connection established"

' Power down
i2c.write(sensor, 0x00)
' Power on
i2c.write(sensor, 0x01)
delay(1000)

' Read one time with low resolution
d = i2c.ReadReg(sensor, 0x23, 2)
ValueLowRes = ((d[0] lshift 8) BOR d[1]) / 1.2
delay(1000)
' Read one time with high resolution
d = i2c.ReadReg(sensor, 0x20, 2)
ValueHighRes = ((d[0] lshift 8) BOR d[1]) / 1.2

print "Low resolution : " + ValueLowRes  + " lx"
print "High resolution: " + valueHighRes + " lx"