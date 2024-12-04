import spi

spi.Open("/dev/spidev0.0")

' Read chip id. BMP280 has id 0x58
id = spi.ReadReg(0xD0, 1)
print "Chip ID:", "0x"; hex(id)  

delay(200)

' Configure sensor
spi.WriteReg(0x75, 0b10000)     ' Set Config: 16x Filter; 0.5ms delay; 4-wire SPI 
spi.WriteReg(0x74, 0b1101111)   ' 4x oversamling temperature; 4x oversampling pressure; normal power mode

' Get calibration data
cal = spi.ReadReg(0x88, 24)
dig_T1 = ByteTo16Bit(cal[0], cal[1])
dig_T2 = Short(ByteTo16Bit(cal[2], cal[3]))
dig_T3 = Short(ByteTo16Bit(cal[4], cal[5]))
dig_P1 = ByteTo16Bit(cal[6], cal[7])
dig_P2 = Short(ByteTo16Bit(cal[8], cal[9]))
dig_P3 = Short(ByteTo16Bit(cal[10], cal[11]))
dig_P4 = Short(ByteTo16Bit(cal[12], cal[13]))
dig_P5 = Short(ByteTo16Bit(cal[14], cal[15]))
dig_P6 = Short(ByteTo16Bit(cal[16], cal[17]))
dig_P7 = Short(ByteTo16Bit(cal[18], cal[19]))
dig_P8 = Short(ByteTo16Bit(cal[20], cal[21]))
dig_P9 = Short(ByteTo16Bit(cal[22], cal[23]))




for ii = 1 to 5
  ' Read Temp
  M = spi.ReadWrite([0xFA], 3)
  T_uncompensated = (M[0] lshift 12) | (M[1] lshift 4) | (M[2] rshift 4)
  T = CalcTemperature(T_uncompensated)
  print "T = "; T


  delay(100)
next

func ByteTo16bit(LSB, MSB)
  return (MSB lshift 8) | LSB
end

func Short(a)
  if(a > 32767) then
    return a - 65536
  else
    return a
  endif
end

func CalcTemperature(adc_T)
  local var1, var2, T

  var1 = (((adc_T rshift 3) - (dig_T1 lshift 1))*dig_T2) rshift 11
  var2 = (((((adc_T rshift 4) - dig_T1) * ((adc_T rshift 4) - dig_T1)) rshift 12) * dig_T3) rshift 14
  t_fine = var1 + var2
  T = (t_fine * 5 + 128) rshift 8
  T = T / 100
  return T
end