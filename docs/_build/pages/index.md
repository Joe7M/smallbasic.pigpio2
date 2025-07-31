---
title: SmallBASIC PiGPIO 2
date: 11.07.2025
---

If you want to let a LED blink or measure the temperature in your room, you can use
[SmallBASIC](https://smallbasic.github.io) on a Raspberry PI. As an example you can
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

__July 11 2025__: SmallBASIC PiGPIO 2 supports now all Pi's from Zero to 5. It can
even run on other LINUX systems. The API got a complete rewrite and is therefore not
anymore compatible with SmallBASIC PiGPIO 1.

## Setup and Pin Numbering

- [Setup SmallBASIC PIGPIO 2](./setup.html)
- [Setup I2C](./setup_i2c.html)
- [Setup SPI](./setup_spi.html)
- [Setup PWM](./setup_pwm.html)
- [Naming and Numbering of the GPIO pins](./rpi_pins.html)

## Function Reference and Examples

- [Generic GPIO](./gpio.html): Digital input and output
- [Generic I2C](./i2c.html): Generic I2C and SMBus interface
- [Generic SPI](./spi.html): Generic SPI interface
- [PWM](./pwm.html): PWM output
- [Framebuffer](./framebuffer.html): Graphics output to the Linux framebuffer
- [SSD1306 compatible OLED Display](./ssd1306.html): Graphics on this a small and cheap b&w OLED display
- [ADS1015 and ADS1115 Voltage Sensor](./ads1x15.html): Measure voltages
- [BMP180](./bmp180.html): Measure temperature and pressure
- [MPU6050](./mpu6050.html): Accelerometer and gyroscope
- [ST7789](./st7789.html): SPI color display
- HD44780: Text display - coming soon
- DS18B20: Temperature sensor - coming soon


## Supported Hardware

- [ADS1015 and ADS1115](./ads1x15.html): Analog to Digital Converter (Voltage Sensor)
- [BH1750](./i2c.html):  Ambient Light Sensor (generic I2C)
- [BMP180](./bmp180.html): Temperature and Pressure Sensor
- [BMP280](./spi.html): Temperature and Pressure Sensor (generic SPI)
- [LED](./gpio.html):  Control a LED (generic GPIO)
- [MPU6050](./mpu6050.html): Accelerometer and Gyroscope
- [Pi Sugar 3 plus](./i2c.html): Battery/USB power supply and charger (generic I2C)
- [Push Button](./gpio.html): Read buttons (generic GPIO)
- [SSD1306](./ssd1306.html): OLED Display
- [ST7789](./st7789.html): SPI color display

## Links

- [SmallBasicPIGPIO at Github](https://github.com/Joe7M/smallbasic.pigpio2)
- [SmallBASIC website](https://smallbasic.github.io/)
- [SmallBASIC forum](https://smallbasic.discourse.group/)
