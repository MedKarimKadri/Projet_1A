#ifndef JOUEUR_H
#define JOUEUR_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"

typedef struct {
    double x, y;        
    double vitesse;    
    int moving_right;  
    int moving_left;    
    int jumping;       
    double jump_velocity; 
    double gravity;    
    int crouching;     
    int attacking;     
    int attack_frame;  
    Uint32 attack_duration; 
    double ground_level; 
    SDL_Surface *spriteSheet;  
    char spriteSheetPath[256]; // Store the path to the sprite sheet
    int frame;          
    Uint32 last_frame_time;
} Perso;

void initialiserPerso(Perso *p, const char *spriteSheetPath);
void movePerso(Perso *p, Uint32 dt);
void renderPerso(SDL_Surface *screen, Perso *p);
void handleJump(Perso *p, Uint32 dt);
void handleCrouch(Perso *p);
void handleAttack(Perso *p, Uint32 dt);

#endif
