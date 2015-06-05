CXX = g++
CC = gcc
CFLAGS = -Wall -c
CXXFLAGS = -Wall -D__STDC_LIMIT_MACROS

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)
OBJECTC = c/tcp.o c/ip.o c/ethernet.o c/icmp.o c/Server.o



CXXSOURCES = \
    main.cpp \
	Server.cpp \
	Client.cpp \
	cpp/Frames/TCPFrame.cpp

all: server

server: 
	$(MAKE) -C c
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -o server $(OBJECTC) $(CXXSOURCES)  -pthread -std=c++11 



ip.o: c/ip.c c/ip.h
	gcc -o c/ip.o -c c/ip.c -W -Wall

tcp.o: c/tcp.c c/tcp.h
	gcc -o c/tcp.o -c c/tcp.c -W -Wall

ethernet.o: c/ethernet.c c/ethernet.h
	gcc -o c/ethernet.o -c c/ethernet.c -W -Wall

Server.o: c/Server.c c/Server.h 
	gcc -o c/Server.o -c c/Server.c -W -Wall

icmp.o: c/icmp.c c/icmp.h
	gcc -o c/icmp.o -c c/icmp.c -W -Wall

.PHONY: clean
clean:
	$(RM) -rf server
	$(RM) ./*.gc??
	$(RM) ./*.o