---
title: Setup PWM on the Raspberry Pi
date: 08.07.2025
---

### On-the-Fly

To load a PWM overlay while Linux is running, execute the following commands in a terminal:

- For single channel PWM: `sudo dtoverlay pwm`
- For dual channel PWM: `sudo dtoverlay pwm-2chan`

Use `sudo dtoverlay -r pwm` to remove the single channel overlay, or 
`sudo dtoverlay -r pwm-2chan` to remove the dual channel overlay.

Overlays loaded with `dtoverlay` stay active until the Raspberry Pi is restarted.

### At Startup

To load a PWM overlay at startup edit the file `/boot/firmware/config.txt` and add
`dtoverlay=pwm` for single channel PWM, or `dtoverlay=pwm-2chan` for dual channel PWM.
Restart the Raspberry Pi to activate the overlay.
