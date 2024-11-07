dnl
dnl Configure script for SmallBasicPIGPIO2 plugins
dnl
dnl This program is distributed under the terms of MIT
dnl

AC_INIT([smallbasicpigpio],[1.0])
AC_CONFIG_AUX_DIR(aux-dist)
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([include/param.cpp])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C])
AC_LANG([C++])
AM_INIT_AUTOMAKE([subdir-objects foreign])

AC_CHECK_LIB(gpiod, gpiod_chip_open_by_name, [], [
        echo "Error! libgpiod not found."
        exit -1
        ])

AC_CHECK_LIB(i2c, i2c_smbus_read_byte_data, [], [
        echo "Error! libi2c not found."
        exit -1
        ])

PLATFORM_LDFLAGS="-Wl,--no-undefined -avoid-version"
GPIO_LDFLAGS="-lgpiod"
I2C_LDFLAGS="-li2c"

AC_SUBST(PLATFORM_LDFLAGS)
AC_SUBST(GPIO_LDFLAGS)
AC_SUBST(I2C_LDFLAGS)

dnl change default aru setting to avoid warning
ARFLAGS=cr
AC_SUBST(ARFLAGS)

LT_INIT([dlopen])
LT_PREREQ([2.2])
AC_PROG_CXX

BUILD_SUBDIRS="spi bmp180 ads1x15 ssd1306 i2c gpio framebuffer"
AC_SUBST(BUILD_SUBDIRS)

libdir=${libdir}/smallbasic

AC_CONFIG_FILES([
Makefile
framebuffer/Makefile
gpio/Makefile
i2c/Makefile
ssd1306/Makefile
ads1x15/Makefile
mpu6050/Makefile
bmp180/Makefile
spi/Makefile])

AC_OUTPUT
