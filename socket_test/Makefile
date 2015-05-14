all: main

main: main.o tcp.o ip.o
	gcc -o main main.o tcp.o ip.o

main.o: main.c tcp.h
	gcc -o main.o -c main.c -W -Wall	

ip.o: ip.c tcp.h ip.h
	gcc -o ip.o -c ip.c -W -Wall

tcp.o: tcp.c tcp.h
	gcc -o tcp.o -c tcp.c -W -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main
