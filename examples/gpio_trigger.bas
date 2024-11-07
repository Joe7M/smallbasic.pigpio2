import gpio

v = 0

gpio.Open()
gpio.SetOutput(21)

for ii = 1 to 10
  gpio.Trigger(21)
  delay(200)
next

print "done"

