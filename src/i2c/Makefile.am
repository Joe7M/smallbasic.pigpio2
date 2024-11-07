# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libi2c.la
libi2c_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp i2c.cpp
libi2c_la_LDFLAGS = -module -rpath '$(libdir)' @I2C_LDFLAGS@ @PLATFORM_LDFLAGS@
