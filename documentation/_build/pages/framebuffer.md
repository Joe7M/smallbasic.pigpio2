---
title: Graphic with Linux Framebuffer
date : 08.11.2024
---

Many SPI-displays provide a framebuffer device (i.e. `/dev/fb0`). 
With SmallBASIC GPIO 2's framebuffer plugin graphics can be displayed
on these displays.
If you don't have a graphical desktop running on your Raspberry Pi and
you are using the terminal instead, the connected monitor is accessible
as a framebuffer device, too.

## Library

To use the framebuffer, import the library `framebuffer`

```SmallBASIC
import framebuffer
```

## Function Reference

### Open Device

```
id = Open()
id = Open(device)
id = Open(device, UseMouse)
id = Open(device, UseMouse, HideText)

```

Open the framebuffer with device name `device` and return a device
id `id`. Serveral framebuffers can be opened at the same time. `id`
is used to select a framebuffer. `UseMouse` can be set to `0`, if 
you don't want to use the mouse. If `HideText` is `1`, the current
text dislay on the console will be hidden. Additionally the blinking
text cursor will be disabled. After ending the program, the console
will be restored.

- `device`
  - String
  - Framebuffer device
  - Parameter is optional. Default value is `"/dev/fb0"`
- `UseMouse`
  - Integer: 0, 1
  - If 1, the mouse will be initialized
  - Optional parameter. Default value is 1.
- `HideText`
  - Integer: 0, 1
  - If 1, console text will be hidden and cursor disabled.
  - Optional parameter. Default value is 1.
- `id`
  - Device id

### Close Device

```
Close(id)
```

Close the framebuffer device with device id `id`.

- `id`
  - Deviced id

### Set Color

```
Color(id, fgcolor)
Color(id, fgcolor, bgcolor)
```

Set foreground color `fgcolor` and background color `bgcolor`. The color
value depends on your framebuffer device.

For example: If you have a 32bit framebuffer with ARGB color order, then
bit 0 to 7 contain the blue channel; bit 8 to 15 the green channel; bit
16 to 23 the red channel and bit 24 to 31 the alpha (transparency) channel.
The standard framebuffer of a screen connected to HDMI is 32bit.

- `id`
  - Deviced id
- `fgcolor`
  - Integer: 32bit
  - Foreground color color
- `bgcolor`
  - Integer: 32bit
  - Background color
  - Optional parameter. Default value is current background color.

Example:

```smallbasic
const BLUE   = 0x000000FF
const GREEN  = 0x0000FF00
const RED    = 0x00FF0000
const YELLOW = 0x00FFFF00
const ORANGE = 0x00FFAA00

Color(id, BLUE)
```

### Clear Screen

```
Clear(id)
Clear(id, color)
```

Clear the screen using color `color`. If color is omitted, the current
background color is used.

- `id`
  - Deviced id
- `color`
  - Integer: 32bit
  - Clear color

### Draw a Pixel

```
SetPixel(id, x1, y1)
SetPixel(id, x1, y1, color)
```

Draw a pixel at point `(x1, y1)` with color `color`

- `id`
  - Deviced id
- `x1, y1`
  - Integer: 0 ... 65535
  - Position in pixel
- `color`
  - Integer: 32bit
  - Line color
  - Optional parameter. Default value is current foreground color

### Draw a Line

```
Line(id, x1, y1, x2, y2)
Line(id, x1, y1, x2, y2, color)
```

- `id`
  - Deviced id
- `x1, y1`
  - Integer: 0 ... 65535
  - Position of start point in pixel
- `x2, y2`
  - Integer: 0 ... 65535
  - Position of end point in pixel
- `color`
  - Integer: 32bit
  - Line color
  - Optional parameter. Default value is current foreground color.

### Draw a Rectangle

```
Rect(id, x1, y1, x2, y2)
Rect(id, x1, y1, x2, y2, color)
Rect(id, x1, y1, x2, y2, color, fill)
```

Draw a rectangle with the top left corner at point `(x1, y1)` and bottom
right corner at `(x2, y2)` with line color `color`. If `fill` is 1 (true),
a filled rectangle will be drawn.

- `id`
  - Deviced id
- `x1, y1`
  - Integer: 0 ... 65535
  - Position of top left corner in pixel
- `x2, y2`
  - Integer: 0 ... 65535
  - Position of bottom right corner in pixel
- `color`
  - Integer: 32bit
  - Line color
  - Optional parameter. Default value is current foreground color.
- `fill`
  - Integer: 0, 1
  - If 1, a filled rectangle will be drawn.
  - Optional parameter. Default value is 0.

### Draw a Rectangle with Round Corners

```
Roundrect(id, x1, y1, x2, y2)
Roundrect(id, x1, y1, x2, y2, radius)
Roundrect(id, x1, y1, x2, y2, radius, color)
Roundrect(id, x1, y1, x2, y2, radius, color, fill)
```

Draw a rectangle with the top left corner at point `(x1, y1)` and bottom
right corner at `(x2, y2)` with line color `color` and rounded corners. The
radius in pixel of the corners if given by `radius`. If `fill` is 1 (true),
a filled rectangle will be drawn.

- `id`
  - Deviced id
- `x1, y1`
  - Integer: 0 ... 65535
  - Position of top left corner in pixel
- `x2, y2`
  - Integer: 0 ... 65535
  - Position of bottom right corner in pixel
- `radius`
  - Integer: 0 ... 255
  - Radius if corners in pixel
  - Optional parameter. Default value is 4.
- `color`
  - Integer: 32bit
  - Line color
  - Optional parameter. Default value is current foreground color
- `fill`
  - Integer: 0, 1
  - If 1, a filled rectangle will be drawn.
  - Optional parameter. Default value is 0.

### Draw a Circle

```
Circle(id, x, y, radius)
Circle(id, x, y, radius, color)
Circle(id, x, y, radius, color, fill)
```

Draw a circle at position `(x, y)` with radius `radius` in pixel. `color`
defines the line color. If `fill` is set to `1` (`true`), then the
circle is filled with `color`. If no color is given, the current foreground
color will be used.

- `id`
  - Deviced id
- `x`
  - Integer: 0 ... 65535
  - X-position in pixel
- `y`
  - Integer: 0 ... 65535
  - Y-position in pixel
- `radius`
  - Integer: 0 ... 65535
  - Radius of the circle in pixel
- `color`
  - Integer: 32bit
  - Line or fill color
  - Optional parameter. Default value is current foreground color.
- `fill`
  - Integer: 0, 1
  - If 1, a filled circle will be drawn
  - Optional parameter. Default value 0

### Draw a Triangle

```
Triangle(id, x1, y1, x2, y2, x3, y3)
Triangle(id, x1, y1, x2, y2, x3, y3, color)
Triangle(id, x1, y1, x2, y2, x3, y3, color, fill)
```

Draw a triangle with the corner points `(x1, y1)`, `(x2, y2)` and
`(x3, y3)` with line color `color`. If `fill` is 1 (true), a filled
triangle will be drawn.

- `id`
  - Deviced id
- `x1, y1`, `x2, y2`, `x3, y3`
  - Integer: 0 ... 65535
  - Position of corner points in pixel
- `color`
  - Integer: 32bit
  - Line color
  - Optional parameter. Default value is current foreground color
- `fill`
  - Integer: 0, 1
  - If 1, a filled rectangle will be drawn.
  - Optional parameter. Default value is 0.

### Print Text

```
Print(id, text)
Print(id, text, color)
```

Print text `text` with text color `color`. After printing the text, the
text cursor advances by one text-line. The following special characters are
supported:

| Character | Description                              |
|:---------:|:-----------------------------------------|
| `\a`      | Set cursor position to upper left (0, 0) |
| `\b`      | Move cursor back by one position         |
| `\n`      | Go to start of current line              |
| `\r`      | Go to line below                         |

- `id`
  - Deviced id
- `text`
  - String
  - Text to be printed
- `color`
  - Integer: 32bit
  - Line color
  - Optional parameter. Default value is current foreground color

### Set Text Cursor Position

```
At(id)
At(id, x)
At(id, x, y)
```

Set the text cursor to the pixel `(x, y)`.

- `id`
  - Deviced id
- `x`
  - Integer: 0 ... 65535
  - X-position in pixel
  - Optional parameter. Default value is `0`
- `y`
  - Integer: 0 ... 65535
  - Y-position in pixel
  - Optional parameter. Default value is `0`

### Set Text Size

```
SetTextSize(id, size)
```

Set text size to `size`. `size` must be an multiple of 8.

- `id`
  - Deviced id
- `size`
  - Integer: 8, 16, 24, 32, ..., 8*n
  - Text size in pixel

### Copy Array to Screen

```
SetArray(id, A)
SetArray(id, A, x)
SetArray(id, A, x, y)
SetArray(id, A, x, y, trans)
```

Copy the content of the 2D-array `A` to screen at position `(x,y)` using
transparency mode `trans`. The following transparency modes are supported:

| Mode | Description                                             |
|:----:|:--------------------------------------------------------|
| 0    | no transparency                                         |
| 1    | Every element of `A` with value `0` will be transparent |

- `id`
  - Deviced id
- `A`
  - 2D-Array of integers: Every element is 32bit long.
  - Every element contains the color of a pixel
- `x` and `y`
  - Integer: 0 ... 65535
  - Position of copied area in pixel
  - Optional parameter. Default value is (0, 0).
- `trans`
  - Integer: 0, 1
  - Transparency mode
  - 0 -> no transparency
  - 1 -> transparancy
  - Optional parameter. Default value is 0.
  
### Copy Screen to Array

```
A = GetArray(id)
A = GetArray(id, x)
A = GetArray(id, x, y)
A = GetArray(id, x, y, w)
A = GetArray(id, x, y, w, h)
```

Copy the screen context inside the rectangle with top-left corner at
`(x, y)`, a width of `w` and a height of `h` to the 2D-array `A`.

- `id`
  - Deviced id
- `A`
  - 2D-Array of integers: Every element is 32bit long
  - Every element contains the color of a pixel
- `x` and `y`
  - Integer: 0 ... 65535
  - Position of top-left corner of copied area in pixel
  - Optional parameter. Default value is (0, 0).
- `w`
  - Integer: 0 ... 65535
  - Width if copy area in pixel
  - Optional parameter. Default value is screen size in x.
- `h`
  - Integer: 0 ... 65535
  - Height if copy area in pixel
  - Optional parameter. Default value is screen size in y.

### Double Buffering

```
SwapBuffer(id)
```

If the framebuffer device supports double buffering, drawing is always performed
in a not visible back-buffer. When calling `SwapBuffer`, back-buffer and front-buffer
will be swapped and the results of the drawing command will be visible.

- `id`
  - Deviced id

### Wait for Vertical Sync

```
WaitForVSync(id)
```

If the framebuffer device supports vertical sync, `WaitForVSync` will wait until the
whole frame is displayed. This can be used to suppress artifacts, when swapping front-
and back-buffer.

- `id`
  - Deviced id

### Get Mouse Data

```
A = GetMouse()
```

Return an 1D-array with current mouse data.

- `A`
  - 1D-array with five integer
  - `A[0]`: x-position of cursor
  - `A[1]`: y-position of cursor
  - `A[2]`: Status of left mouse button
  - `A[3]`: Status of middle mouse button
  - `A[4]`: Status of right mouse button

## Example

```smallbasic
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

FB.Close(id)
```