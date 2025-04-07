#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>

#define PUZZLE_WIDTH 4
#define PUZZLE_HEIGHT 4
#define TILE_SIZE 256
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define CHRONO_FRAME_WIDTH 100
#define CHRONO_FRAME_HEIGHT 100
#define CHRONO_MARGIN 20
#define TOTAL_TIME 240 // 4 minutes en secondes

typedef struct {
    SDL_Surface *image;
    SDL_Rect tiles[PUZZLE_WIDTH][PUZZLE_HEIGHT];
    int emptyTileX;
    int emptyTileY;
} Puzzle;

void chargerImages(SDL_Surface **image, SDL_Surface **chronoSheet, SDL_Surface **victoire, SDL_Surface **echec);
void genererPuzzle(Puzzle *puzzle, SDL_Surface *image);
void afficherPuzzle(SDL_Surface *screen, Puzzle *puzzle, SDL_Surface *chronoSheet, int chronoIndex);
void resoudreAvecSouris(Puzzle *puzzle, int x, int y);
int puzzleResolu(Puzzle *p);

#endif
