CC = gcc
CFLAGS = -Wall -Wextra -g
SDL_FLAGS = $(shell sdl-config --cflags --libs) -lSDL_image -lSDL_ttf -lSDL_mixer

all: prog

prog: main.o fonction.o
	$(CC) -o prog main.o fonction.o $(SDL_FLAGS)

main.o: main.c fonction.h joueur.h
	$(CC) $(CFLAGS) -c main.c

fonction.o: fonction.c fonction.h
	$(CC) $(CFLAGS) -c fonction.c

clean:
	rm -f *.o prog

