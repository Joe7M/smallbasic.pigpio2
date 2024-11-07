import gpio

v = 0

gpio.Open()
gpio.SetInput(21)

while(1)
  print gpio.Read(21)
  delay(500)
wend

print "done"

