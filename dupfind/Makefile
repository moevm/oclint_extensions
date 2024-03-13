.PHONY: all install clean uninstall

CC=g++

all: dupfind

dupfind: main.cpp
	$(CC) main.cpp -o dupfind

clean:
	rm -f dupfind

install: dupfind
	cp dupfind /usr/local/bin

uninstall:
	rm -f /usr/local/bin/dupfind
