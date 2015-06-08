CXX = g++
CC = gcc
CFLAGS = -Wall -c 
CXXFLAGS = -Wall -D__STDC_LIMIT_MACROS

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)




CXXSOURCES = \
    main.cpp \
	cpp/Frames/TCPFrame.cpp \
	cpp/Stack.cpp \
	cpp/Frames/ETHFrame.cpp \
	cpp/Frames/ARPFrame.cpp \
	cpp/Frames/IPFrame.cpp \
	cpp/Frames/ICMPFrame.cpp \
	cpp/Client.cpp \
	cpp/PC.cpp

all: server

server: 
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -o server  $(CXXSOURCES)  -pthread -std=c++11  -fdiagnostics-color



.PHONY: clean
clean:
	$(RM) -rf server
	$(RM) ./c/*.gc??
	$(RM) ./c/*.o
	$(RM) ./*.o