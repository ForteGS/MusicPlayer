CC=g++
CFLAGS=-g -O2 -Wall $(shell sdl2-config --cflags) --libs
LDFLAGS=$(shell sdl2-config --cflags --libs) -lSDL_mixer -lsmpeg2

msplayer.o: main.cpp
	$(CC) -c `sdl2-config --cflags` $(pkg-config --cflags libvlc) main.cpp
	$(CC) -o msplayer main.o `sdl2-config --cflags --libs` -lSDL2_mixer -lsmpeg2 -lvlc

clean:
	rm -f *.o
