---
title: SmallBASIC PiGPIO 2
date: 08.11.2024
---

If you want to let a LED blink or measure the temperature in your room, you can use
[SmallBASIC](https://smallbasic.github.io) on a Raspberry PI. As an example yiu can
connect a LED and a resistor to a GPIO-Pin of your Raspberry Pi, write a short
SmallBASIC programm and enjoy the blinking LED:

```smallbasic
import gpio

const PIN_GPIO4 = 4

gpio.SetOutput(PIN_GPIO4)

for ii = 1 to 5
	gpio.Write(PIN_GPIO4, 1)
	delay(500)
	gpio.Write(PIN_GPIO4, 0)
	delay(500)
next
```

![Blinking LED](./images/blink.gif)


## News

__December 14 2024__: SmallBASIC PiGPIO 2 supports now all Pi's from Zero to 5. It could
even run on other LINUX systems. The API got a complete rewrite and is therefore not
anymore compatible with SmallBASIC PiGPIO 1.

## Setup and Pin numbering

- [Setup of SmallBASIC PIGPIO 2](./setup.html)
- [Setup I2C](./setupi2c.html)
- [Setup SPI](./setupspi.html)
- [Setup PWM](./gpio_numbering.html)
- [Naming and Numbering of the GPIO pins](./gpio_numbering.html)

## Topics

- [Generic GPIO](./gpio.html): Digital input and output
- [Generic I2C](./i2c.html): Generic I2C and SMBus interface
- [Generic SPI](./spi.html): Generic SPI interface
- [Framebuffer](./framebuffer.html): Graphics output to the Linux framebuffer
- [SSD1306 compatible OLED Display](./ssd1306.html): Graphics on this a small and cheap b&w OLED display
- [BH1750 Ambient Light Sensor](./bh1750.html): Measure ambient light
- [ADS1015 and ADS1115 Voltage Sensor](./ads1x15.html): Measure voltages
- [BMP180](./bmp180.html): Measure temperature and pressure
- [MPU6050](./mpu6050.html): Accelerometer and gyroscope

## Examples

- [LED](./example_led.html)
- [Push Button](./example_pushbutton.html)
- [DS18B20 Temperature Sensor](./example_ds18b20.html)
- [HD44780 Text LCD Display](./example_hd44780.html)
- [SSD1306 OLED Display](./example_ssd1306.html)
- [BH1750 Ambient Light Sensor](./example_bh1750.html)
- [ADS1015 and ADS1115 Voltage Sensor](./example_ads1x15.html)
- [SCD30 CO2, Temperature and Humidity Sensor](./example_scd30.html)

## Links

- [SmallBasicPIGPIO at Github](https://github.com/Joe7M/SmallBasicPIGPIO)
- [Feedback for SmallBasicPIGPIO](https://github.com/Joe7M/SmallBasicPIGPIO/discussions)
- [SmallBASIC website](https://smallbasic.github.io/)
- [SmallBASIC forum](https://www.syntaxbomb.com/smallbasic/)
