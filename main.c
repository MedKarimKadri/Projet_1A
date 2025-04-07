#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "enigme.h"
#include "intro.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Puzzle Game", NULL);

    SDL_Surface *image, *chronoSheet, *victoire, *echec;
    chargerImages(&image, &chronoSheet, &victoire, &echec);
    
    Puzzle puzzle;
    genererPuzzle(&puzzle, image);
    
    SDL_Event event;
    intro(screen);
    
    Uint32 startTime = SDL_GetTicks();
    int running = 1;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            } else if(event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x / TILE_SIZE;
                int y = event.button.y / TILE_SIZE;
                resoudreAvecSouris(&puzzle, x, y);
            }
        }

        Uint32 currentTime = SDL_GetTicks() - startTime;
        int seconds = currentTime / 1000;
        int frameIndex = seconds / 20; // Change toutes les 20 secondes


        if(seconds >= TOTAL_TIME) {
            SDL_BlitSurface(echec, NULL, screen, NULL);
            SDL_Flip(screen);
            SDL_Delay(3000);
            running = 0;
        }


        afficherPuzzle(screen, &puzzle, chronoSheet, frameIndex);


        if(puzzleResolu(&puzzle)) {
            SDL_BlitSurface(victoire, NULL, screen, NULL);
            SDL_Flip(screen);
            SDL_Delay(3000);
            running = 0;
        }

        SDL_Delay(50);
    }


    SDL_FreeSurface(image);
    SDL_FreeSurface(chronoSheet);
    SDL_FreeSurface(victoire);
    SDL_FreeSurface(echec);
    SDL_Quit();
    return 0;
}
