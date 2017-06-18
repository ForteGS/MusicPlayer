CC=g++
CFLAGS=-g -O2 -Wall $(shell sdl-config --cflags)
LDFLAGS=$(shell sdl-config --libs) -lSDL_mixer

msplayer.o: main.cpp
	$(CC) -c `sdl2-config --cflags` main.cpp
	$(CC) -o msplayer main.o `sdl2-config --libs` -lSDL2_mixer

clean:
	rm -f *.o
