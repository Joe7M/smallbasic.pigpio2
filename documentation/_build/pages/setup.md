---
title: Installation Instructions
date: 20.06.2025
---

### Setup the Raspberry Pi

1. Install the newest Raspberry Pi OS on a SD card. 
2. Insert the SD card and boot your Raspberry Pi. Setup the desktop, login to wifi and install your favorite software.
3. You can find a beginners guide here: [www.raspberrypi.org](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up/0). 

### Install SmallBASIC

Right now there aren't any SmallBasic packages or executable files for the Raspberry Pi. You have to compile SmallBASIC by yourself. Don't be afraid, it is fairly easy.

Open a console and copy paste the following commands one at a time to build the SDL2-version:

```
cd ~
sudo apt update
sudo apt dist-upgrade
sudo apt install autotools-dev automake libsdl2-dev libfreetype6-dev libfontconfig1-dev xxd
wget https://github.com/smallbasic/SmallBASIC/releases/download/12_27/smallbasic-12.27.tar.gz
tar -xvzf smallbasic-12.27.tar.gz
cd smallbasic-12.27
export CC=gcc
export PACKAGE_LIBS=-latomic
./configure --enable-sdl
make
sudo make install
```

The `make` command will take quite a while. Don't get inpatient. After you finished the installation, you can type `sbasicg` in the console and SmallBASIC should start.

Usually it is quite nice to use the console version of SmallBASIC, especially if you don't have any graphical output to the computer screen. To build the console version copy and paste the following commands one at a time after you build the SDL2-version:

```
./configure
make
sudo make install
```

### Install SmallBASIC PIGPIO 2

Open again a console and copy paste the following commands one at a time:

```
sudo apt install libtool libgpiod-dev libi2c-dev
cd ~
git clone https://github.com/Joe7M/smallbasic.pigpio2.git
cd smallbasic.pigpio2/src
sh ./autogen.sh
./configure
make
sudo make install
```

The libraries have been installed in `/usr/local/lib/smallbasic`.

### Start SmallBASIC with the plugin

Start a console and type:

```
sbasicg -m/usr/local/lib/smallbasic -r 'file'
```

For example you want to execute `led.bas` in the example folder:

```
cd ~
cd smallbasic.pigpio2/examples
sbasicg -m/usr/local/lib/smallbasic -r gpio_led.bas
```

Now you can start coding your own programs. :)

