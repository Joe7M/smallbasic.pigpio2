# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libgpio.la
libgpio_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp gpio.cpp
libgpio_la_LDFLAGS = -module -rpath '$(libdir)' @GPIO_LDFLAGS@ @PLATFORM_LDFLAGS@
