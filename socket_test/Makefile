all: main

main: main.o socket.o
	gcc -o main main.o socket.o

main.o: main.c socket.h
	gcc -o main.o -c main.c -W -Wall	

socket.o: socket.c socket.h
	gcc -o socket.o -c socket.c -W -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main
