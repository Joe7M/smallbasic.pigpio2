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

print "done"

