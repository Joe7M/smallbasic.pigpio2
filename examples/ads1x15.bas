' Example on how to measure analog voltage signals with a ADS1015 or
' ADS1115 sensor using SmallBASIC PiGPIO plugin
' Tested with SmallBASIC 12.28
' By Joerg Siebenmorgen
' MIT Licence, 2024
'
' Console version: 	sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin ads1x15.bas
' SDL version: 		sudo sbasicg -m/home/pi/SmallBasicPIGPIO/bin -r ads1x15.bas
'

import ads1x15 as adc

const A0  = 4   ' Input A0
const A1  = 5   ' Input A1
const A2  = 6   ' Input A2
const A3  = 7   ' Input A3
const A01 = 0   ' Differential Input A0 - A1
const A03 = 1   ' Differential Input A0 - A3
const A13 = 2   ' Differential Input A1 - A3
const A23 = 3   ' Differential Input A2 - A3


adc.Open("/dev/i2c-1", 0x48)    ' Open device on address 0x48
adc.Channel(A0)                 ' Set input channel A0
adc.VoltageRange(6.144)         ' Set Voltage range from 0 to 6.144V
adc.SampleRate(128)             ' 128 Samples per second


for ii = 1 to 10
    delay(500)
    print adc.Read()            ' Returns voltage as float
next

adc.Close()                     ' Close connection
