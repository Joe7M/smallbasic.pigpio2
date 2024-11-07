# I2C

## Command Reference

### Library

Access the basic I2C command with the library `i2c`:

```SmallBASIC
import i2c
```

### Open device

```
id = Open(address)
id = Open(address, interface)
```

__address__ = 0 ... 127 : I2C device address.  
__interface__: String containing the I2C interface name. Default value: `/dev/i2c-1`  
__id__: device id

Open a connected I2C device with address `address` on I2C interface `interface`.
`interface` is an optional parameter. Several devices can be opened. `id` will be
used to identify device.

Example:

```
id = open(0x55)
id = open(0x55, "/dev/i2c-1")
```

### Close device

```
Close(id)
```

__id__: Device id

When closing a SmallBASIC program, I2C access will be automatically closed. If you want
to manually close I2C access for the device with device id `id`, use this function.

### Write to I2C device

```
Write(id, dataByte)
Write(id, dataArray)
```

__id__: device id  
__dataByte__ = 0 ... 255: one byte of data  
__dataArray__ = [0 ... 255, 0 ... 255, ... ]: 1D-array of bytes

Send one byte `dataByte` or an 1d array `dataArray` of bytes to the devices with device id `id`.

Example:

```
Write(id, 0x20)
Write(id, [0x20, 0x21, 0xA3])
```

### Write to register of a I2C device 

```
WriteReg(id, reg, dataByte)
WriteReg(id, reg, dataArray)
```

__id__: device id  
__reg__ = 0 ... 255: register  
__dataByte__ = 0 ... 255: one byte of data  
__dataArray__ = [0 ... 255, 0 ... 255, ... ]: 1D-array of bytes

Send one byte `dataByte` or an 1d array `dataBytes` of bytes to the devices with device id `id`.

Example:

```
WriteReg(id, 0x05, 0x20)
WriteReg(id, 0x05, [0x20, 0x21, 0xA3])
```

### Read from I2C device

```
res = Read(id)
res = Read(id, bytes)
```

__id__: device id  
__bytes__: number of bytes to read from device. Default value is `1`  
__res__: data read from device. if more than 1 byte is read, `res` is an 1d array.

Read number of bytes `bytes` from the device with device id `id`. `bytes` is an optional parameter.

### Read register of I2C device

```
res = Read(id, reg)
res = Read(id, reg, bytes)
```

__id__: device id  
__reg__ = 0 ... 255: register  
__bytes__: number of bytes to read from device. Default value is `1`  
__res__: data read from device. if more than 1 byte is read, `res` is an 1d array.

Read number of bytes `bytes` from the register `reg` of the device with device id `id`. `bytes` is an optional parameter.

Example:

```
res = ReadReg(id, 0x05)       ' Read one byte from register 0x05
res = ReadReg(id, 0x05, 5)    ' Read five bytes from register 0x05
```

### Read byte from SMBus device

```
res = SmbusReadByte(id, reg)
```

__id__: device id  
__reg__ = 0 ... 255: register  
__res__: data read from device.

Read one byte from the register `reg` of the SMBus device with device id `id`.

Example:

```
res = SmbusReadByte(id, 0x05)       ' Read one byte from register 0x05
```

### Read word from SMBus device

```
res = SmbusReadWord(id, reg)
```

__id__: device id  
__reg__ = 0 ... 255: register  
__res__: data read from device.

Read one word (2 bytes) from the register `reg` of the SMBus device with device id `id`.

Example:

```
res = SmbusReadWord(id, 0x05)       ' Read one byte from register 0x05
```

### Write byte to SMBus device

```
SmbusWriteByte(id, reg, data)
```

__id__: device id  
__reg__ = 0 ... 255: register  
__data__ = 0 ... 255: data

Send one byte to the register `reg` of the SMBus device with device id `id`.

Example:

```
SmbusWriteByte(id, 0x05, 0xA2)       ' Write 0xA2 to register 0x05
```

### Write word to SMBus device

```
SmbusWriteWord(id, reg, data)
```

__id__: device id  
__reg__ = 0 ... 255: register  
__data__ = 0 ... 65535: data

Send one word (2 bytes) to the register `reg` of the SMBus device with device id `id`.

Example:

```
SmbusWriteWord(id, 0x05, 0xA202)       ' Write 0xA202 to register 0x05
```

## Examples

### 1. BH1750 - Ambient Light Sensor

```SmallBASIC
import i2c

Print "Connect to BH1750"

sensor = i2c.Open(0x23, "/dev/i2c-1")

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
```

### 2. PiSugar 3 Plus - Power Supply

```SmallBASIC
import i2c

Print "Access PiSugar 3 Plus"

PiSugar = i2c.Open(0x57, "/dev/i2c-1")

ChargeStatus = i2c.SmbusReadByte(PiSugar, 0x2A)
print ChargeStatus; "%"

VoltageHigh = i2c.SmbusReadByte(PiSugar, 0x22)
VoltageLow = i2c.SmbusReadByte(PiSugar, 0x23)
Voltage = (VoltageHigh lshift 8) + VoltageLow
print Voltage; "mV"

IsPowerConnected = i2c.SmbusReadByte(PiSugar, 0x02) rshift 7
print "USB Power connected: "; IsPowerConnected

print "Seconds of RTC: "; i2c.SmbusReadByte(PiSugar, 0x37)
```
