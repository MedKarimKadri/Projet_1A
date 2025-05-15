#include "fonction.h"
#include <stdio.h>

Image initialiserImage(char *chemin) {
    Image img;
    img.surface = IMG_Load(chemin);
    if (!img.surface) {
        printf("Erreur chargement image : %s\n", SDL_GetError());
        img.surface = NULL;
        return img;
    }

    // Position de l'image
    img.position.x = 0;
    img.position.y = 0;

    return img;
}

void afficherImage(SDL_Surface *ecran, Image img) {
    SDL_BlitSurface(img.surface, NULL, ecran, &img.position);
}

void libererImage(Image *img) {
    if (img->surface) {
        SDL_FreeSurface(img->surface);
        img->surface = NULL;
    }
}

Texte initialiserTexte(char *contenu, char *font_path, int taille, SDL_Color couleur, int x, int y) {
    Texte txt;
    txt.police = TTF_OpenFont(font_path, taille);
    if (!txt.police) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
    }
    txt.couleur = couleur;
    strcpy(txt.texte, contenu);
    txt.surface = TTF_RenderText_Solid(txt.police, txt.texte, txt.couleur);
    txt.position.x = x;
    txt.position.y = y;
    return txt;
}

void afficherTexte(SDL_Surface *ecran, Texte txt) {
    SDL_BlitSurface(txt.surface, NULL, ecran, &txt.position);
}

void libererTexte(Texte *txt) {
    if (txt->surface) {
        SDL_FreeSurface(txt->surface);
        txt->surface = NULL;
    }
    if (txt->police) {
        TTF_CloseFont(txt->police);
    }
}

Bouton initialiserBouton(char *normal, char *survole, char *son_path, int x, int y) {
    Bouton btn;
    btn.images[0] = IMG_Load(normal);
    btn.images[1] = IMG_Load(survole);
    btn.position.x = x;
    btn.position.y = y;
    btn.son = Mix_LoadWAV(son_path); // Charger le son ici
    if (!btn.son) {
        printf("Erreur de chargement du son : %s\n", Mix_GetError());
    }
    btn.etat = 0;
    return btn;
}

void afficherBouton(SDL_Surface *ecran, Bouton btn) {
    SDL_BlitSurface(btn.images[btn.etat], NULL, ecran, &btn.position);
}

void libererBouton(Bouton *btn) {
    SDL_FreeSurface(btn->images[0]);
    SDL_FreeSurface(btn->images[1]);
    Mix_FreeChunk(btn->son); // Libérer le son ici
}
