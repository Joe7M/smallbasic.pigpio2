import spi

spi.Open("/dev/spidev0.0")

' Read chip id. BMP280 has id 0x58
id = spi.ReadReg(0xD0, 1)
print "Chip ID:", "0x"; hex(id)  

delay(500)

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

' Configure sensor
spi.WriteReg(0x74, 0b01101111)     ' 011: 4x oversampling temperature; 011: 4x oversampling pressure; 11: normal power mode
spi.WriteReg(0x75, 0b01010000)     ' 010: t_standby = 125ms;  100: 5 Samples to reach 75%; 00: 4-wire SPI

for ii = 1 to 5
  M = Measure()
  print "T = "; M[0]; " °C   P = "; round(M[1]/100,2);" hPa"
  delay(200)
next


'#############################################################################

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

func Measure()
  local var1, var2, T, M, T_uncompensated, t_fine, P_uncompensated, P

  ' Get temperature
  M = spi.ReadReg(0xFA, 3)  
  T_uncompensated = (M[0] lshift 12) | (M[1] lshift 4) | (M[2] rshift 4)  
  

  var1 = (((T_uncompensated rshift 3) - (dig_T1 lshift 1))*dig_T2) rshift 11
  var2 = (((((T_uncompensated rshift 4) - dig_T1) * ((T_uncompensated rshift 4) - dig_T1)) rshift 12) * dig_T3) rshift 14
  t_fine = var1 + var2
  T = (t_fine * 5 + 128) rshift 8
  T = T / 100

  ' Get pressure
  M = spi.ReadWrite([0xF7], 3)
  
  P_uncompensated = (M[0] lshift 12) | (M[1] lshift 4) | (M[2] rshift 4) 

  var1 = t_fine - 128000
  var2 = var1 * var1 * dig_P6
  var2 = var2 + ((var1 * dig_P5) lshift 17)
  var2 = var2 + (dig_P4 lshift 35)
  var1 = ((var1 * var1 * dig_P3) rshift 8) + ((var1 * dig_P2) lshift 12)
  var1 = (((1 lshift 47) + var1)) * dig_P1 rshift 33
  if (var1 == 0) then return 0
  P = 1048576 -  P_uncompensated
  P = (((P lshift 31) - var2) * 3125) / var1
  var1 = (dig_P9 * (P rshift 13) * (P rshift 13)) rshift 25
  var2 = (dig_P8 * P) rshift 19
  P = ((P + var1 + var2) rshift 8) + (dig_P7 lshift 4)
  P = P/256
  return [T, P]
end

