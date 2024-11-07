# GPIO

## Library

To access the gpio pins import the library `gpio`:

```SmallBASIC
import gpio
```

## Command Reference

### Open

```
Open()
Open(chipname)
```

__chipname__: String containing the gpio chip. Default value is `gpiochip0`

Open gpio access to the chip specified by the optional parameter `chipname`.
If `chipname` is not provided, the standart gpiochip of the Raspberry Pi will be
opened.

### SetInput

```
SetInput(PinNumber)
```

__PinNumber__ = 0 ... 53

Sets GPIO pin `PinNumber` as an input. The internal pullup-resistor will be
set automaticly. Before using a GPIO pin, you should use either `SetInput` or
`SetOutput`.

### SetOutput

```
SetOutput(PinNumber)
```

__PinNumber__ = 0 ... 53

Sets GPIO pin `PinNumber` as an output. Before using a GPIO pin, you should
use either `SetInput` or `SetOutput`.

### Write

```
Write(Pin, Level)
```

__Pin__ = 0 ... 53  
__Level__ = 0 or 1  

If `Pin` is configured as output, the voltage level `level` can be set to ground
(low) or +3.3V (high).

Example:

```
Write(4, 0)   ' Sets pin 4 to low (ground)
Write(4, 1)   ' Sets pin 4 to high (+3.3V)
```

### Read

```
Status = Read(Pin)
```

__Pin__ = 0 ... 53  
__Status__ = 0 or 1

If `Pin` is configured as input, the voltage level can be read. If the specified pin
is at ground (low), `Status` will be `1`. If the pin is at +3.3V (high) `Status` will be 0.

Example:

```
Status = Read(4)   ' Reads the voltage level of pin 4
```

### Trigger

```
Trigger(Pin)
Trigger(Pin, Duration)
Trigger(Pin, Duration, Level)
```

__Pin__ = 0 ... 53  
__Duration__ = 1 ... 100  
__Level__ = 0 or 1

If pin `Pin` is configured as output, a trigger pulse at this pin will be emitted with a
`Duration` in microseconds and a voltage level `Level` of low or high.

Example:

```
Trigger(4, 50, 1)  ' A 50 µs Trigger pulse at pin 4 with +3.3V (high)
```

### Wait for Trigger

```
Status = WaitForTrigger(Pin)
Status = WaitForTrigger(Pin, Timeout)
```

__Pin__ = 0 ... 53  
__Timeout__ = 1 ... 255 seconds; default value: 1
__Status__ = -1, 0, 1: Return value

Wait for a raising edge event at the specified pin `Pin`. `Timeout` is in seconds.
Maximum timeout is 255s. `Timeout` an optional parameter.

Return value:

| Value | Status               |
|:-----:|:---------------------|
| -1    | error                |
|  0    | time out             |
|  1    | rising edge detected |


Example:

```
Trigger(4, 50, 1)  'A 50 µs Trigger pulse at pin 4 with +3.3V (high)
```


### Close

```
Close()
```

When closing a SmallBASIC program, gpio access will be automatically closed. If you want
to manually close gpio access, use this function.

## Examples

### 1. LED

```SmallBASIC
import gpio

gpio.Open()
gpio.SetOutput(18)

for ii = 1 to 10
  v = !v
  gpio.Write(18, v)
  delay(100)
next
```

### 2. Button

```SmallBASIC
import gpio

v = 0

gpio.Open()
gpio.SetInput(21)

while(1)
  print gpio.Read(21)
  delay(500)
wend
```

### 3. Trigger

```SmallBASIC
import gpio

v = 0

gpio.Open()
gpio.SetOutput(21)

for ii = 1 to 10
  gpio.Trigger(21)
  delay(200)
next
```

### 4. Wait for Trigger

```SmallBASIC
import gpio

v = 0

gpio.Open()
gpio.SetInput(21)

result = gpio.WaitTrigger(21, 5)

select case result
  case  0: print "Time out"
  case  1: print "Rising edge detected"
  case -1: print "Error"
end select
```
