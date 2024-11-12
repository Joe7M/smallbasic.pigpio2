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

BUILD_SUBDIRS=""

AC_CHECK_LIB(gpiod, gpiod_chip_open_by_name, [have_gpiod="yes"], [have_gpiod="no"])
if test "${have_gpiod}" = "no" ; then
    AC_MSG_WARN([libgpiod v1.6. not installed: building without gpio support.])
else
    GPIO_LDFLAGS="-lgpiod"
    AC_SUBST(GPIO_LDFLAGS)
    BUILD_SUBDIRS="${BUILD_SUBDIRS} gpio"
fi


AC_CHECK_LIB(i2c, i2c_smbus_read_byte_data, [have_i2c="yes"], [have_i2c="no"])
if test "${have_i2c}" = "no" ; then
    AC_MSG_WARN([libi2c not installed: building without I2C support.])
else
    I2C_LDFLAGS="-li2c"
    AC_SUBST(I2C_LDFLAGS)
    BUILD_SUBDIRS="${BUILD_SUBDIRS} i2c bmp180 ads1x15 ssd1306"
fi

PLATFORM_LDFLAGS="-Wl,--no-undefined -avoid-version"
AC_SUBST(PLATFORM_LDFLAGS)


dnl change default aru setting to avoid warning
ARFLAGS=cr
AC_SUBST(ARFLAGS)

LT_INIT([dlopen])
LT_PREREQ([2.2])
AC_PROG_CXX

BUILD_SUBDIRS="${BUILD_SUBDIRS} spi framebuffer"
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
