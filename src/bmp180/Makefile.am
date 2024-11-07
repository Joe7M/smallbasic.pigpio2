# SmallBasicPIGPIO2
#
# This program is distributed under the terms of MIT

AM_CXXFLAGS=-fno-rtti -std=c++14
AM_CPPFLAGS = -I../include -Wall
lib_LTLIBRARIES = libbmp180.la
libbmp180_la_SOURCES = ../include/param.cpp ../include/hashmap.cpp main.cpp bmp180.cpp
libbmp180_la_LDFLAGS = -module -rpath '$(libdir)' @PLATFORM_LDFLAGS@
