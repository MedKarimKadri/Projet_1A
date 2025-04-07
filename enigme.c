#include "enigme.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void chargerImages(SDL_Surface **image, SDL_Surface **chronoSheet, SDL_Surface **victoire, SDL_Surface **echec) {
    *image = IMG_Load("puzzle.png");
    *chronoSheet = IMG_Load("chrono_small.png");
    *victoire = IMG_Load("victoire.png");
    *echec = IMG_Load("echec.png");

    if (!*image || !*chronoSheet || !*victoire || !*echec) {
        printf("Erreur de chargement des images: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void genererPuzzle(Puzzle *puzzle, SDL_Surface *image) {
    puzzle->image = image;
    

    for (int y = 0; y < PUZZLE_HEIGHT; y++) {
        for (int x = 0; x < PUZZLE_WIDTH; x++) {
            puzzle->tiles[x][y].x = x * TILE_SIZE;
            puzzle->tiles[x][y].y = y * TILE_SIZE;
            puzzle->tiles[x][y].w = TILE_SIZE;
            puzzle->tiles[x][y].h = TILE_SIZE;
        }
    }


    srand(time(NULL));
    puzzle->emptyTileX = PUZZLE_WIDTH - 1;
    puzzle->emptyTileY = PUZZLE_HEIGHT - 1;
    
    for(int i = 0; i < 1000; i++) {
        int dir = rand() % 4;
        int newX = puzzle->emptyTileX;
        int newY = puzzle->emptyTileY;
        
        switch(dir) {
            case 0: newX++; break;
            case 1: newX--; break;
            case 2: newY++; break;
            case 3: newY--; break;
        }
        
        if(newX >= 0 && newX < PUZZLE_WIDTH && newY >= 0 && newY < PUZZLE_HEIGHT) {
            SDL_Rect temp = puzzle->tiles[newX][newY];
            puzzle->tiles[newX][newY] = puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY];
            puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY] = temp;
            puzzle->emptyTileX = newX;
            puzzle->emptyTileY = newY;
        }
    }
}

void afficherPuzzle(SDL_Surface *screen, Puzzle *puzzle, SDL_Surface *chronoSheet, int chronoIndex) {
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));


    for (int y = 0; y < PUZZLE_HEIGHT; y++) {
        for (int x = 0; x < PUZZLE_WIDTH; x++) {
            if (x == puzzle->emptyTileX && y == puzzle->emptyTileY) continue;
            
            SDL_Rect dest = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_BlitSurface(puzzle->image, &puzzle->tiles[x][y], screen, &dest);
        }
    }


    if(chronoIndex >= 0 && chronoIndex < 12) {
        SDL_Rect src = {
            chronoIndex * CHRONO_FRAME_WIDTH,
            0,
            CHRONO_FRAME_WIDTH,
            CHRONO_FRAME_HEIGHT
        };
        
        SDL_Rect dest = {
            SCREEN_WIDTH - CHRONO_FRAME_WIDTH - CHRONO_MARGIN,
            CHRONO_MARGIN,
            CHRONO_FRAME_WIDTH,
            CHRONO_FRAME_HEIGHT
        };

        SDL_BlitSurface(chronoSheet, &src, screen, &dest);
    }

    SDL_Flip(screen);
}

void resoudreAvecSouris(Puzzle *puzzle, int x, int y) {
    if(x < 0 || x >= PUZZLE_WIDTH || y < 0 || y >= PUZZLE_HEIGHT) return;

    if((abs(x - puzzle->emptyTileX) == 1 && y == puzzle->emptyTileY) ||
       (abs(y - puzzle->emptyTileY) == 1 && x == puzzle->emptyTileX)) {
        SDL_Rect temp = puzzle->tiles[x][y];
        puzzle->tiles[x][y] = puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY];
        puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY] = temp;
        puzzle->emptyTileX = x;
        puzzle->emptyTileY = y;
    }
}

int puzzleResolu(Puzzle *p) {
    for(int y = 0; y < PUZZLE_HEIGHT; y++) {
        for(int x = 0; x < PUZZLE_WIDTH; x++) {
            if(p->tiles[x][y].x != x * TILE_SIZE || p->tiles[x][y].y != y * TILE_SIZE) {
                return 0;
            }
        }
    }
    return 1;
}
