CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
SHELL=		bash

all:		lib_info

lib_info:	lib_info.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f lib_info
