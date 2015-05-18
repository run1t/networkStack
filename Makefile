CXX = g++
CC = gcc
CFLAGS = -Wall -c
CXXFLAGS = -Wall -D__STDC_LIMIT_MACROS

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)
OBJECTC = socket_test/tcp.o socket_test/ip.o socket_test/ethernet.o socket_test/icmp.o socket_test/Server.o



CXXSOURCES = \
    main.cpp \
	Server.cpp \
	Client.cpp

all: server

server: 
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -o server $(OBJECTC) $(CXXSOURCES)  -pthread -std=c++11 



ip.o: socket_test/ip.c socket_test/ip.h
	gcc -o socket_test/ip.o -c socket_test/ip.c -W -Wall

tcp.o: socket_test/tcp.c socket_test/tcp.h
	gcc -o socket_test/tcp.o -c socket_test/tcp.c -W -Wall

ethernet.o: socket_test/ethernet.c socket_test/ethernet.h
	gcc -o socket_test/ethernet.o -c socket_test/ethernet.c -W -Wall

Server.o: socket_test/Server.c socket_test/Server.h 
	gcc -o socket_test/Server.o -c socket_test/Server.c -W -Wall

icmp.o: socket_test/icmp.c socket_test/icmp.h
	gcc -o socket_test/icmp.o -c socket_test/icmp.c -W -Wall

.PHONY: clean
clean:
	$(RM) -rf server
	$(RM) ./*.gc??
	$(RM) ./*.o