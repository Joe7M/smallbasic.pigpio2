# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libads1x15.la
libads1x15_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp ads1x15.cpp
libads1x15_la_LDFLAGS = -module -rpath '$(libdir)' @PLATFORM_LDFLAGS@
