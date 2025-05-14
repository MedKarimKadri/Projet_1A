/**
 * @file enigme.h
 * @brief En-tête pour la gestion du puzzle
 * @author Karim
 * @version 1.0
 * @date Mai 11, 2025
 */

#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>

#define PUZZLE_WIDTH 3  /**< Largeur du puzzle en tuiles */
#define PUZZLE_HEIGHT 3  /**< Hauteur du puzzle en tuiles */
#define TILE_SIZE 341  /**< Taille d'une tuile en pixels */
#define SCREEN_WIDTH 1024  /**< Largeur écran */
#define SCREEN_HEIGHT 1024  /**< Hauteur écran */
#define CHRONO_FRAME_WIDTH 100  /**< Largeur frame chrono */
#define CHRONO_FRAME_HEIGHT 100  /**< Hauteur frame chrono */
#define CHRONO_MARGIN 20  /**< Marge chrono */
#define TOTAL_TIME 120  /**< Temps total */

/**
 * @struct Puzzle
 * @brief Structure contenant l'état du puzzle
 */
typedef struct {
    SDL_Surface *image; /**< Image du puzzle */
    SDL_Rect tiles[PUZZLE_WIDTH][PUZZLE_HEIGHT]; /**< Grille des tuiles */
    int emptyTileX; /**< Position X case vide */
    int emptyTileY; /**< Position Y case vide */
} Puzzle;

/* Prototypes des fonctions */
void chargerImages(SDL_Surface **image, SDL_Surface **chronoSheet, SDL_Surface **victoire, SDL_Surface **echec);
void genererPuzzle(Puzzle *puzzle, SDL_Surface *image);
void afficherPuzzle(SDL_Surface *screen, Puzzle *puzzle, SDL_Surface *chronoSheet, int chronoIndex);
void resoudreAvecSouris(Puzzle *puzzle, int x, int y);
void gererChrono(int *chronoIndex, Uint32 elapsedSeconds, Puzzle *p, int *estVictoire, int *estEchec);
void afficherEffetRotoZoom(SDL_Surface *screen, SDL_Surface *image, Uint32 startTime, Uint32 currentTime);

#endif
