# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libssd1306.la
libssd1306_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp ssd1306.cpp ../framebuffer/FBdraw.cpp
libssd1306_la_LDFLAGS = -module -rpath '$(libdir)' @PLATFORM_LDFLAGS@
