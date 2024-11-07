import i2c

Print "Connect to MPU-6050"
sensor = i2c.open(0x68, "/dev/i2c-1")
Print "Connection established"

delay(500)

WhoamI = i2c.ReadReg(sensor, 0x75)
print "WHO_AM_I: ", hex(WhoamI)     ' Check for connection: sensor returns 0x68


'i2c.WriteReg(sensor, 0x19, 0x00)

'i2c.WriteReg(sensor, 0x1A, 0x00)

'i2c.WriteReg(sensor, 0x1B, 0x08)

'i2c.WriteReg(sensor, 0x1C, 0x00)

'i2c.WriteReg(sensor, 0x6B, 0x01)

' SMPLRT_DIV
i2c.Write(sensor, [0x19,0x00])
' MPU config
i2c.Write(sensor, [0x1A,0x00])
' Gyro config
i2c.Write(sensor, [0x1B,0x08])
' Accel config
i2c.Write(sensor, [0x1C,0x00])
' Turn on
i2c.Write(sensor, [0x6B,0x01])

for ii = 1 to 1000
    
    A = GetAcceleration()
    G = GetGyroscope()
    T = GetTemperature()

    locate 5,0
    print "Acc: [";
    print USING "##.00 "; A.AccX, A.AccY, A.AccZ;
    print "]  Gryo: [";
    print USING "####.00 "; G.GyrX, G.GyrY, G.GyrZ;
    print "] Temp : ";
    print USING "##.00 "; T

    delay(100)
    showpage
next
    
    
func GetAcceleration()
    local A
    dim A
    
    A.AccX = short((i2c.ReadReg(sensor, 0x3B) lshift 8) BOR i2c.ReadReg(sensor, 0x3C)) / 16384
    A.AccY = short((i2c.ReadReg(sensor, 0x3D) lshift 8) BOR i2c.ReadReg(sensor, 0x3E)) / 16384
    A.AccZ = short((i2c.ReadReg(sensor, 0x3F) lshift 8) BOR i2c.ReadReg(sensor, 0x40)) / 16384
    
    return A
end

func GetGyroscope()
    local d
    dim d
    
    d.GyrX = short((i2c.ReadReg(sensor, 0x43) lshift 8) BOR i2c.ReadReg(sensor, 0x44)) / 65.5
    d.GyrY = short((i2c.ReadReg(sensor, 0x45) lshift 8) BOR i2c.ReadReg(sensor, 0x46)) / 65.5
    d.GyrZ = short((i2c.ReadReg(sensor, 0x47) lshift 8) BOR i2c.ReadReg(sensor, 0x48)) / 65.5
    
    return d
end

func GetTemperature()
    return short((i2c.ReadReg(sensor, 0x41) lshift 8) BOR i2c.ReadReg(sensor, 0x42)) / 340 + 36.53
end

func CalculateAccelerationAngle(AccX, AccY, AccZ)
    local d
    dim d
    
    d.AngleAccX = atan2(AccY, sqr(AccZ^2 + AccX^2)) * 360 / 2.0 / PI
    d.AngleAccY = atan2(AccX, sqr(AccZ^2 * AccY^2)) * 360 / -2.0 / PI
    
    return d
end

func short(dat)
    if dat > 32767 then
        return dat - 65536
    else
        return dat
    endif
end

