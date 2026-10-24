import hd44780 as lcd

const GPIO_DB4 = 0     ' Pin 27
const GPIO_DB5 = 5     ' Pin 29
const GPIO_DB6 = 6     ' Pin 31
const GPIO_DB7 = 13    ' Pin 33
const GPIO_E   = 19    ' Pin 35
const GPIO_RS  = 26    ' Pin 37

lcd.open(GPIO_DB4, GPIO_DB5, GPIO_DB6, GPIO_DB7, GPIO_E, GPIO_RS)

lcd.cls()

s = "SmallBASIC is a fast and easy to learn BASIC language interpreter ideal for everyday calculations, scripts and prototypes. SmallBASIC includes trigonometric, matrices and algebra functions, a built in IDE, a powerful string library, system, sound, graphic commands and communication via serial or sockets along with structured programming syntax."
l = len(s)
ii = 0

lcd.print("-= SmallBASIC =-")

while(1)
  ii++
  s1 = mid(s, ii, 16)
  lcd.locate(1,2)
  lcd.print s1
  delay(500)
  if(ii == l) then ii = 0
wend

print "Done"
