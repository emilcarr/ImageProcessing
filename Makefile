PREFIX=/usr/local/

CC=g++
CFLAGS=-pthread -lboost_program_options -lm -lX11

all: pixelsorting itoascii
default: all

pixelsorting:
	$(CC) pixelsorting.cpp -o pixelsorting $(CFLAGS)

itoascii:
	$(CC) itoascii.cpp -o itoascii $(CFLAGS)

install: all
	mkdir -p $(PREFIX)/bin
	cp pixelsorting $(PREFIX)/bin
	cp itoascii $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/pixelsorting
	rm -f $(PREFIX)/bin/itoascii

clean:
	rm -f pixelsorting itoascii
