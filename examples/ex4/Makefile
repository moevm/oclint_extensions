.PHONY: all

all: prog

prog: stuff.o main.o
	gcc stuff.o main.o

stuff.o: src/stuff.c include/obfuscation.h include/stuff.h
	gcc -c -I ./include src/stuff.c

main.o: src/main.c include/stuff.h
	gcc -c -I ./include src/main.c