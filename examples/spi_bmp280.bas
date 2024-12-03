import spi

spi.Open("/dev/spidev0.0")

delay(100)
id = spi.ReadReg(0xD0, 1)
print "Chip ID", hex(id)

spi.Write([0x75, 0b10000])
spi.Write([0x74, 0b1101111])

print "0xF4", spi.ReadReg(0xF4, 1)
print "0xF5", spi.ReadReg(0xF5, 1)

for ii = 1 to 5
  'T1 = spi.ReadReg(0xFA, 1)
  'T2 = spi.ReadReg(0xFB, 1)
  'T3 = spi.ReadReg(0xFC, 1)
  T = spi.ReadWrite([0xFA], 3)
  print T
  delay(100)
next