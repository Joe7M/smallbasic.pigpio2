# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libhd44780.la
libhd44780_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp hd44780.cpp gpio.cpp
libhd44780_la_LDFLAGS = -module -rpath '$(libdir)' @GPIO_LDFLAGS@ @PLATFORM_LDFLAGS@
