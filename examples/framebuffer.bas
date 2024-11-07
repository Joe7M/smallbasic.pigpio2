OPTION BASE 1

import Framebuffer as FB

FILL = true
TRANSPARENT = true

dim a(100,100)

for xx = 1 to 100
    for yy = 1 to 100
        a[xx,yy] = floor(rnd*2^32)
    next
next

for xx = 10 to 40
    for yy = 10 to 40
        a[xx,yy] = 0
    next
next

id = FB.Open("/dev/fb0")

StartTime = ticks()

FB.Clear(id)
FB.Color(id, 0x0000FF00)
FB.SetPixel(id, 10,10)
FB.SetPixel(id, 20,10, 0x0000FFFF)
FB.setPixel(id, 30,10, 0x000000FF)

FB.Line(id, 50,10,100,60, 0x00FFFFFF)

FB.Rect(id, 110,10,160,60, 0x00AA8899, FILL)
FB.Rect(id, 110,10,160,60, 0x00AA4477)

FB.RoundRect(id, 170,10,220,60, 4, 0x00AA8899, FILL)
FB.RoundRect(id, 170,10,220,60, 4, 0x00AA4477)

FB.Circle(id, 255,35,20, 0x00AA8899, FILL)
FB.Circle(id, 255,35,20, 0x00AA4477)

FB.Triangle(id, 280,60, 305,10, 330,60, 0x00AA8899, FILL)
FB.Triangle(id, 280,60, 305,10, 330,60,0x00AA4477)

FB.At(id, 10, 70)
FB.Print(id, "test1")
FB.SetTextSize(id, 16)
FB.At(id, 10, 90)
FB.Print(id, "test2", 0x00118833)

FB.SetArray(id, a, 10, 120, TRANSPARENT)

'x,y,w,h
b = FB.GetArray(id, 10, 120, 50, 50)
FB.SetArray(id, b, 120, 120)

FB.WaitForVSync(id)
delay(1)
FB.SwapBuffer(id)

ElapsedTime = ticks() - StartTime

print "Time per frame: " + ElapsedTime/60 + "ms"

FB.Close(id)
