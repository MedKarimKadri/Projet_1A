/**
 * @file main.c
 * @brief Programme principal du jeu
 * @author Karim
 * @version 1.0
 * @date Mai 11, 2025
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "enigme.h"

/**
 * @brief Point d'entrée principal
 * @param argc Nombre arguments
 * @param argv Tableau arguments
 * @return Code de sortie
 */
int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Puzzle Game", NULL);

    SDL_Surface *image, *chronoSheet, *victoire, *echec;
    Puzzle puzzle;
    SDL_Event event;
    Uint32 startTime, currentTime;
    int running = 1, estVictoire = 0, estEchec = 0, frameIndex, seconds, x, y;
    Uint32 effectStartTime = 0;
    int showingEffect = 0;

    chargerImages(&image, &chronoSheet, &victoire, &echec);
    genererPuzzle(&puzzle, image);
    
    startTime = SDL_GetTicks();

    while(running) {
        currentTime = SDL_GetTicks() - startTime;
        seconds = currentTime / 1000;
        
        gererChrono(&frameIndex, seconds, &puzzle, &estVictoire, &estEchec);
        
	if (!estVictoire && !estEchec) {
	afficherPuzzle(screen, &puzzle, chronoSheet, frameIndex);
	} 
	else {
	if (!showingEffect) {
		effectStartTime = SDL_GetTicks();
		showingEffect = 1;
	}
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
 	   SDL_Surface *effectImage = estVictoire ? victoire : echec;
	afficherEffetRotoZoom(screen, effectImage, effectStartTime, SDL_GetTicks());
	}


        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            } 
            else if(event.type == SDL_MOUSEBUTTONDOWN && !estVictoire && !estEchec) {
                x = event.button.x / TILE_SIZE;
                y = event.button.y / TILE_SIZE;
                resoudreAvecSouris(&puzzle, x, y);
            }
        }

        if (showingEffect && (SDL_GetTicks() - effectStartTime >= 3000)) {
            running = 0;
        }

        SDL_Flip(screen);
        SDL_Delay(50);
    }

    SDL_FreeSurface(image);
    SDL_FreeSurface(chronoSheet);
    SDL_FreeSurface(victoire);
    SDL_FreeSurface(echec);
    SDL_Quit();
    return 0;
}
