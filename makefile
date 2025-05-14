prog: main.o enigmetf.o
	gcc -o prog main.o enigmetf.o -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lm
main.o:main.c
	gcc -c main.c  -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -g
enigmetf.o:enigmetf.c
	gcc -c enigmetf.c -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -g
