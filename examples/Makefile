CXX = g++
CXXFLAGS = -Wall 


CXXSOURCES = \
	../lib/Frames/TCPFrame.cpp \
	../lib/Frames/ETHFrame.cpp \
	../lib/Frames/ARPFrame.cpp \
	../lib/Frames/IPFrame.cpp \
	../lib/Frames/ICMPFrame.cpp \
	../lib/Stack.cpp \
	../lib/PC.cpp \
	../lib/Connection.cpp 

all: server HttpServer Client

server: 
	$(CXX) $(CXXFLAGS) -o server  $(CXXSOURCES) ../lib/Server.cpp Server/Server.cpp  -std=c++11  -fdiagnostics-color

HttpServer: 
	$(CXX) $(CXXFLAGS) -o httpServer  $(CXXSOURCES) ../lib/Server.cpp HttpServer/HttpServer.cpp  -std=c++11  -fdiagnostics-color

Client: 
	$(CXX) $(CXXFLAGS) -o client  $(CXXSOURCES) ../lib/Client.cpp Client/Client.cpp  -std=c++11  -fdiagnostics-color

.PHONY: all server HttpServer Client clean
clean:
	$(RM) -rf server
	$(RM) -rf httpServer
	$(RM) -rf client
	$(RM) ./*.o