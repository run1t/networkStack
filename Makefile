CXX = g++
CC = gcc
CFLAGS = -Wall -c 
CXXFLAGS = -Wall -D__STDC_LIMIT_MACROS

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)




CXXSOURCES = \
    main.cpp \
	lib/Frames/TCPFrame.cpp \
	lib/Stack.cpp \
	lib/Frames/ETHFrame.cpp \
	lib/Frames/ARPFrame.cpp \
	lib/Frames/IPFrame.cpp \
	lib/Frames/ICMPFrame.cpp \
	lib/Frames/SendFrame.cpp \
	lib/Client.cpp \
	lib/PC.cpp \
	lib/Connection.cpp 

all: server

server: 
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -o server  $(CXXSOURCES)  -pthread -std=c++11  -fdiagnostics-color



.PHONY: clean
clean:
	$(RM) -rf server
	$(RM) ./*.o