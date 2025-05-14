prog: main.o enigme.o
	gcc -o prog main.o enigme.o -lSDL -lSDL_image -lSDL_gfx -no-pie
main.o: main.c enigme.h
	gcc -c main.c
enigme.o: enigme.c enigme.h
	gcc -c enigme.c
clean:
	rm -f prog *.o
