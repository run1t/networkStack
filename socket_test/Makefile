all: main

main: main.o tcp.o ip.o ethernet.o Server.o
	gcc -o main main.o tcp.o ip.o ethernet.o Server.o -fdiagnostics-color=auto

main.o: main.c 
	gcc -o main.o -c main.c -W -Wall	

ip.o: ip.c ip.h
	gcc -o ip.o -c ip.c -W -Wall

tcp.o: tcp.c tcp.h
	gcc -o tcp.o -c tcp.c -W -Wall

ethernet.o: ethernet.c ethernet.h
	gcc -o ethernet.o -c ethernet.c -W -Wall

Server.o: Server.c Server.h 
	gcc -o Server.o -c Server.c -W -Wall -fdiagnostics-color=auto

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main
