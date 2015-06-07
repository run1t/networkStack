CXX = g++
CC = gcc
CFLAGS = -Wall -c 
CXXFLAGS = -Wall -D__STDC_LIMIT_MACROS

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)
OBJECTC = c/tcp.o c/ip.o c/ethernet.o c/icmp.o c/Server.o



CXXSOURCES = \
    main.cpp \
	cpp/Frames/TCPFrame.cpp \
	cpp/Stack.cpp \
	cpp/Frames/ETHFrame.cpp \
	cpp/Frames/IPFrame.cpp \
	cpp/PC.cpp

all: server

server: 
	$(MAKE) -C c 
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -o server $(OBJECTC) $(CXXSOURCES)  -pthread -std=c++11  -fdiagnostics-color



.PHONY: clean
clean:
	$(RM) -rf server
	$(RM) ./c/*.gc??
	$(RM) ./c/*.o
	$(RM) ./*.o