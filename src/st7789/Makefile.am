# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libst7789.la
libst7789_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp st7789.cpp ../framebuffer/FBdraw.cpp gpio.cpp spi.cpp
libst7789_la_LDFLAGS = -module -rpath '$(libdir)' @GPIO_LDFLAGS@ @PLATFORM_LDFLAGS@
