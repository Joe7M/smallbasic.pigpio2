# SmallBASIC PIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libpwm.la
libpwm_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp pwm.cpp
libpwm_la_LDFLAGS = -module -rpath '$(libdir)' @GPIO_LDFLAGS@ @PLATFORM_LDFLAGS@
