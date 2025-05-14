/**
 * @file enigme.c
 * @brief Implémentation des fonctions du puzzle
 * @author Karim
 * @version 1.0
 * @date Mai 11, 2025
 */

#include "enigme.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Charge les images du jeu
 * @param image Surface du puzzle
 * @param chronoSheet Spritesheet du chrono
 * @param victoire Image de victoire
 * @param echec Image d'échec
 * @return Nothing
 */
void chargerImages(SDL_Surface **image, SDL_Surface **chronoSheet, SDL_Surface **victoire, SDL_Surface **echec) {
    *image = IMG_Load("puzzle.png");
    *chronoSheet = IMG_Load("chrono_small.png");
    *victoire = IMG_Load("victoire.png");
    *echec = IMG_Load("echec.png");
}

/**
 * @brief Initialise le puzzle
 * @param puzzle Structure à initialiser
 * @param image Image source
 * @return Nothing
 */
void genererPuzzle(Puzzle *puzzle, SDL_Surface *image) {
    int x, y, i, dir, newX, newY;
    SDL_Rect temp;

    puzzle->image = image;
    
    for (y = 0; y < PUZZLE_HEIGHT; y++) {
        for (x = 0; x < PUZZLE_WIDTH; x++) {
            puzzle->tiles[x][y].x = x * TILE_SIZE;
            puzzle->tiles[x][y].y = y * TILE_SIZE;
            puzzle->tiles[x][y].w = TILE_SIZE;
            puzzle->tiles[x][y].h = TILE_SIZE;
        }
    }

    srand(time(NULL));
    puzzle->emptyTileX = PUZZLE_WIDTH - 1;
    puzzle->emptyTileY = PUZZLE_HEIGHT - 1;
    
    for(i = 0; i < 1000; i++) {
        dir = rand() % 4;
        newX = puzzle->emptyTileX;
        newY = puzzle->emptyTileY;
        
        switch(dir) {
            case 0: newX++; break;
            case 1: newX--; break;
            case 2: newY++; break;
            case 3: newY--; break;
        }
        
        if(newX >= 0 && newX < PUZZLE_WIDTH && newY >= 0 && newY < PUZZLE_HEIGHT) {
            temp = puzzle->tiles[newX][newY];
            puzzle->tiles[newX][newY] = puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY];
            puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY] = temp;
            puzzle->emptyTileX = newX;
            puzzle->emptyTileY = newY;
        }
    }
}

/**
 * @brief Affiche le puzzle à l'écran
 * @param screen Surface d'affichage
 * @param puzzle État du puzzle
 * @param chronoSheet Spritesheet du chrono
 * @param chronoIndex Index animation chrono
 * @return Nothing
 */
void afficherPuzzle(SDL_Surface *screen, Puzzle *puzzle, SDL_Surface *chronoSheet, int chronoIndex) {
    int x, y;
    SDL_Rect src, dest;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    for (y = 0; y < PUZZLE_HEIGHT; y++) {
        for (x = 0; x < PUZZLE_WIDTH; x++) {
            if (x == puzzle->emptyTileX && y == puzzle->emptyTileY) continue;
            
            dest.x = x * TILE_SIZE;
            dest.y = y * TILE_SIZE;
            dest.w = TILE_SIZE;
            dest.h = TILE_SIZE;
            SDL_BlitSurface(puzzle->image, &puzzle->tiles[x][y], screen, &dest);
        }
    }

    if(chronoIndex >= 0 && chronoIndex < 12) {
        src.x = chronoIndex * CHRONO_FRAME_WIDTH;
        src.y = 0;
        src.w = CHRONO_FRAME_WIDTH;
        src.h = CHRONO_FRAME_HEIGHT;
        
        dest.x = SCREEN_WIDTH - CHRONO_FRAME_WIDTH - CHRONO_MARGIN;
        dest.y = CHRONO_MARGIN;
        dest.w = CHRONO_FRAME_WIDTH;
        dest.h = CHRONO_FRAME_HEIGHT;

        SDL_BlitSurface(chronoSheet, &src, screen, &dest);
    }
}

/**
 * @brief Gère les déplacements de tuiles
 * @param puzzle Puzzle à modifier
 * @param x Position X cliquée
 * @param y Position Y cliquée
 * @return Nothing
 */
void resoudreAvecSouris(Puzzle *puzzle, int x, int y) {
    SDL_Rect temp;

    if(x < 0 || x >= PUZZLE_WIDTH || y < 0 || y >= PUZZLE_HEIGHT) return;

    if((abs(x - puzzle->emptyTileX) == 1 && y == puzzle->emptyTileY) ||
       (abs(y - puzzle->emptyTileY) == 1 && x == puzzle->emptyTileX)) {
        temp = puzzle->tiles[x][y];
        puzzle->tiles[x][y] = puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY];
        puzzle->tiles[puzzle->emptyTileX][puzzle->emptyTileY] = temp;
        puzzle->emptyTileX = x;
        puzzle->emptyTileY = y;
    }
}

/**
 * @brief Gère le chronomètre et l'état du jeu
 * @param chronoIndex Index frame chrono
 * @param elapsedSeconds Temps écoulé
 * @param p Puzzle à vérifier
 * @param estVictoire Flag victoire
 * @param estEchec Flag échec
 * @return Nothing
 */
void gererChrono(int *chronoIndex, Uint32 elapsedSeconds, Puzzle *p, int *estVictoire, int *estEchec) {
    int x, y;

    *chronoIndex = (elapsedSeconds % TOTAL_TIME) / 10;
    if(*chronoIndex >= 12) *chronoIndex = 11;
    
    *estVictoire = 1;
    for(y = 0; y < PUZZLE_HEIGHT; y++) {
        for(x = 0; x < PUZZLE_WIDTH; x++) {
            if(p->tiles[x][y].x != x * TILE_SIZE || p->tiles[x][y].y != y * TILE_SIZE) {
                *estVictoire = 0;
                break;
            }
        }
        if(!*estVictoire) break;
    }
    
    if(elapsedSeconds >= TOTAL_TIME) *estEchec = 1;
}

/**
 * @brief Affiche un effet de rotozoom
 * @param screen Surface d'affichage
 * @param image Image à animer
 * @param startTime Début animation
 * @param currentTime Temps actuel
 * @return Nothing
 */
void afficherEffetRotoZoom(SDL_Surface *screen, SDL_Surface *image, Uint32 startTime, Uint32 currentTime) {
    Uint32 elapsed = currentTime - startTime;
    float progress = (float)elapsed / 3000.0f;

    if (progress > 1.0f) progress = 1.0f;

    double angle = progress * 720.0;
    double zoom = 0.5 + progress * 1.5;

    SDL_Surface *rotated = rotozoomSurface(image, angle, zoom, 1);
    if (rotated) {
        SDL_Rect dest = {
            (SCREEN_WIDTH - rotated->w) / 2,
            (SCREEN_HEIGHT - rotated->h) / 2
        };
        SDL_BlitSurface(rotated, NULL, screen, &dest);
        SDL_FreeSurface(rotated);
    }
}
