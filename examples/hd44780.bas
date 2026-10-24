import hd44780 as lcd

const GPIO_DB4 = 0     ' Pin 27
const GPIO_DB5 = 5     ' Pin 29
const GPIO_DB6 = 6     ' Pin 31
const GPIO_DB7 = 13    ' Pin 33
const GPIO_E   = 19    ' Pin 35
const GPIO_RS  = 26    ' Pin 37

lcd.open(GPIO_DB4, GPIO_DB5, GPIO_DB6, GPIO_DB7, GPIO_E, GPIO_RS)

lcd.cls()

lcd.print("  Hello  World")
lcd.locate(1,2)
lcd.print("-= SmallBASIC =-")
print "Done"
