all: hello
hello: main.o header.o
	gcc -o CosSim main.o header.o -lm
main.o: main.c
	gcc -Wall -ansi -c main.c header.c 
header.o: header.c header.h
	gcc -c header.c
clean:
	rm -rf *o CosSim