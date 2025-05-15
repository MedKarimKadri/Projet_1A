#ifndef FONCTION_H
#define FONCTION_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>

// Structure pour gérer une image
typedef struct {
    SDL_Surface *surface;
    SDL_Rect position;
} Image;

typedef struct {
    TTF_Font *police;
    SDL_Surface *surface;
    SDL_Rect position;
    SDL_Color couleur;
    char texte[255];
} Texte;

typedef struct {
    SDL_Surface *images[2]; // [0] = normal, [1] = survolé/cliqué
    SDL_Rect position;
    Mix_Chunk *son;
    int etat; // 0 = normal, 1 = survolé/cliqué
} Bouton;

Image initialiserImage(char *chemin);
void afficherImage(SDL_Surface *ecran, Image img);
void libererImage(Image *img);

Texte initialiserTexte(char *contenu, char *police, int taille, SDL_Color couleur, int x, int y);
void afficherTexte(SDL_Surface *ecran, Texte txt);
void libererTexte(Texte *txt);

Bouton initialiserBouton(char *imageNormal, char *imageHover, char *son, int x, int y);
void afficherBouton(SDL_Surface *ecran, Bouton btn);
void libererBouton(Bouton *btn);

#endif
