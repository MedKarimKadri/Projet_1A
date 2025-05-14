/**
 * @file main.c
 * @brief Programme principal du jeu d’énigmes en SDL.
 * @author Salma
 * @version 0.1
 * @date 14 mai 2025
 */
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "enigmetf.h"

/**
 * @brief Point d'entrée du jeu : initialisation SDL, affichage, boucle principale.
 * 
 * Ce programme affiche une énigme avec trois réponses. Le joueur clique pour répondre.
 * Une animation de victoire ou d’échec est affichée selon la réponse. Le jeu s'arrête après 3 mauvaises réponses ou une bonne réponse.
 * 
 * @return int Code de retour du programme.
 */
int main() {
	SDL_Surface* ecran = NULL;
	Enigme e;
	SDL_Event event;
 	int running = 1;
	int reponse = -1;


	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	ecran = SDL_SetVideoMode(1400, 933, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    	SDL_Surface* background = IMG_Load("bck.jpg");
	remplir_fichier_questions();
	initialiser_enigme(&e, ecran);
	generer_question_aleatoire(&e);
	e.debut_chrono = SDL_GetTicks();

	while(running) {
	SDL_BlitSurface(background, NULL, ecran, NULL);

	if((e.afficher_victoire || e.afficher_echec) && !e.showingEffect) {
		e.effectStartTime = SDL_GetTicks();
		e.showingEffect = 1;
		e.effectImage = e.afficher_victoire ? e.victoire : e.echec;
	}

	if(!e.afficher_victoire && !e.afficher_echec) {
		afficher_enigme(&e, ecran);
		afficher_chronometre(&e, ecran);
	}
	else {
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		afficherEffetRotoZoom(ecran, e.effectImage, e.effectStartTime, SDL_GetTicks());
	}

	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) running = 0;
		if(event.type == SDL_MOUSEBUTTONDOWN && !e.afficher_victoire && !e.afficher_echec) {
			int x = event.button.x;
			int y = event.button.y;
			if(y >= 380 && y <= 480) {
				if(x >= 200 && x <= 500) reponse = 0;
				else if(x >= 520 && x <= 820) reponse = 1;
				else if(x >= 840 && x <= 1140) reponse = 2;
			}
		}
	}

	if(reponse != -1) {
		if(resoudre_enigme(&e, reponse)) {
			e.afficher_victoire = 1;
		} else {
			e.afficher_echec = 1;
			e.vies--;
		}
		reponse = -1;
	}

	if(gerer_chrono(e.debut_chrono) >= TEMPS_MAX && !e.afficher_victoire && !e.afficher_echec) {
		e.afficher_echec = 1;
		e.vies--;
	}

	if(e.showingEffect) {
		if(SDL_GetTicks() - e.effectStartTime >= 3000) {
			if(e.afficher_victoire) {
				running = 0;
			}
		else {
			if(e.vies <= 0) running = 0;
		else {
			e.afficher_echec = 0;
			e.showingEffect = 0;
			generer_question_aleatoire(&e);
			e.debut_chrono = SDL_GetTicks();
			}
		}
		}
	}
	SDL_Flip(ecran);
	SDL_Delay(10);
	}

	SDL_FreeSurface(background);
 	SDL_FreeSurface(e.chrono_sheet);
	SDL_FreeSurface(e.victoire);
	SDL_FreeSurface(e.echec);
	TTF_CloseFont(e.police);
	SDL_Quit();
 
	return 0;
}
