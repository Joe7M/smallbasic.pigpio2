---
title: "ADS1X15 - Analog to Digital Converter"
date : 07.11.2024
---

## Library

To use the ADS1x15, import the library `ads1x15`

```SmallBASIC
import ads1x15
```

## Function Reference

### Open Device

```
Open()
Open(interface)
Open(interface, address)
Open(interface, address, version)
```

Open the ADS1X15 on interface `interface` with I2C address `address` and device type `version`.


- `interface`
  - String
  - Path to I2C-Interface.
  - Optional: default value is `"/dev/i2c-1"`
- `address`
  - Integer: 0 ... 255
  - Device address
  - Optional: default value is `0x48`
- `version`
  - Integer: 0 or 1
  - ADS1115 -> `version = 1`
  - ADS1015 -> `version = 0`
  - Optional: default value is `1`

Example:

```SmallBASIC
Open("/dev/i2c-1", 0x48, 1)
```

### Set Channel

```
Channel(n)
```

Set channel for measurement. 

- `n`
  - Integer: 0 ... 7

    | n | Type         | Pin     |
    |:-:|:------------:|:-------:|
    | 0 | differential | A0 - A1 | 
    | 1 | differential | A0 - A3 |
    | 2 | differential | A1 - A3 |
    | 3 | differential | A2 - A3 |
    | 4 | single-ended | A0      |
    | 5 | single-ended | A1      |
    | 6 | single-ended | A2      |
    | 7 | single-ended | A3      |

### Set Voltage Range

```
VoltageRange()
VoltageRange(V)
```

Set the voltage range in Volts. Any value less than the minimum value will
set the minimum voltage range. Any value greater than the maximum value
will set the maximum voltage range. The first voltage range greater than
or equal the the specified value will be set.

- `V`
  - Float: 0.256, 0.512, 1.024, 2.048, 4.096 or 6.144

Example:

```SamallBASIC
VoltageRange(1024)  ' Set voltage range to 1.024V
VoltageRange(1025)  ' Set voltage range to 2.048V
```

### Set Sample Rate

```
SampleRate()
SampleRate(S)
```

Set the sample rate in samples/second. Any value less than the minimum value
will set the minimum sample range. Any value greater than the maximum value
will set the maximum sample range. The first sample range greater than or
equal the the specified value will be set.

- `S`
  - Integer
    - ADS1015: 128, 250, 490, 1600, 2400, 3300
    - ADS1115: 8, 16, 32, 64, 128, 250, 475, 860
    - Optional: default value is 128

Example:

```SmallBASIC
SampleRate(128)
```

### Read Voltage

```
V = Read()
```

Measure the voltage.

- `V`
  - Float
  - Measured voltage in Volts



### Set continious mode

```
ContiniousMode()
```

Set ADS1X15 to continious measurement mode. ADS1x15 will measure with the
specified sample rate. The read-command will return the value of the last
measurement.


### Set single shot mode

```
SingleShotMode()
```

Set ADS1x15 to single shot mode. Each read-command will initiate a
measurement.

### Close device

```
Close()
```

Close the ADS1X15 device.


## SmallBASIC Example

```SmallBasic
import ads1x15 as adc

const A0  = 4   ' Input A0
const A1  = 5   ' Input A1
const A2  = 6   ' Input A2
const A3  = 7   ' Input A3
const A01 = 0   ' Differential Input A0 - A1
const A03 = 1   ' Differential Input A0 - A3
const A13 = 2   ' Differential Input A1 - A3
const A23 = 3   ' Differential Input A2 - A3


adc.Open("/dev/i2c-1", 0x48)    ' Open device
adc.Channel(A0)                 ' Set input channel A0
adc.VoltageRange(6.144)         ' Set Voltage range from 0 to 6.144V
adc.SampleRate(128)             ' 128 Samples per second


for ii = 1 to 10
    delay(500)
    print adc.Read()            ' Returns voltage as float
next

adc.Close()                     ' Close connection
```
