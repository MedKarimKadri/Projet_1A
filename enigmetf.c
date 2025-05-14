/**
 * @file enigmetf.c
 * @brief Implémentation des fonctions liées au jeu d'énigmes SDL (affichage, logique, effets).
 */
#include "enigmetf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

/**
 * @brief Remplit le fichier texte avec un ensemble fixe de questions et réponses.
 */
void remplir_fichier_questions() {
	FILE* f = fopen("questions.txt", "w");
	if (f) { 
	fprintf(f, "Quelle planete est surnommee 'la Planete Rouge'?\nVenus\nMars\nJupiter\n1\nQuel est le plus grand type d'etoile dans l'univers?\nNaine blanche\nEtoile a neutrons\nHypergeante\n2\nCombien de temps faut-il a la lumiere du Soleil pour atteindre la Terre?\n8 minutes\n1 heure\n3 secondes\n0\nQuel element est le plus abondant dans le Soleil?\nHelium\nHydrogene\nOxygene\n1\nQuelle est la vitesse de liberation necessaire pour quitter la Terre?\n7.9 km/s\n15.3 km/s\n11.2 km/s\n2\nComment s'appelle notre galaxie?\nVoie Lactee\nAndromede\nSombrero\n0\nQuel phenomene spatial absorbe toute lumiere environnante?\nSupernova\nNebuleuse\nTrou noir\n2\nQuelle est la temperature moyenne a la surface de Venus?\n462°C\n-63°C\n15°C\n0\nQuel satellite naturel possede des eruptions cryovolcaniques?\nLune\nEncelade\nIo\n1\nQuelle est la duree d'une annee sur Jupiter?\n12 ans terrestres\n4 ans terrestres\n88 ans terrestres\n0");
	fclose(f);
	}
}

/**
 * @brief Initialise la structure Enigme avec les ressources graphiques et questions.
 * @param e Structure à initialiser.
 * @param ecran Surface SDL principale.
 */
void initialiser_enigme(Enigme* e, SDL_Surface* ecran) {
	if(e->chrono_sheet == NULL) {	
		e->chrono_sheet = IMG_Load("chrono.png");
		e->police = TTF_OpenFont("space.ttf", 60);
		e->victoire = IMG_Load("victoire.png");
		e->echec = IMG_Load("echec.png");
		e->vies = 3;
		e->afficher_echec = 0;
		e->afficher_victoire = 0;

		FILE* f = fopen("questions.txt", "r");
		if(f) {
			for(int i=0; i<MAX_QUESTIONS; i++) {
				fgets(e->questions[i].question, 256, f);
				for(int j=0; j<3; j++) {
					fgets(e->questions[i].reponses[j], 256, f);
				}
			fscanf(f, "%d\n", &e->questions[i].bonne_reponse);
			e->questions_utilisees[i] = 0;
			}
		fclose(f);
		}
	srand(time(NULL));
	}
}

/**
 * @brief Sélectionne une question aléatoire non posée.
 * @param e Pointeur vers structure Enigme.
 */
void generer_question_aleatoire(Enigme* e) {
	int index;
	do {
	index = rand() % MAX_QUESTIONS;
	} while(e->questions_utilisees[index]);
    
	e->questions_utilisees[index] = 1;
	strcpy(e->question, e->questions[index].question);
	for(int i=0; i<3; i++) {
		strcpy(e->reponses[i], e->questions[index].reponses[i]);
	}
	e->bonne_reponse = e->questions[index].bonne_reponse;
	e->debut_chrono = SDL_GetTicks();
}

/**
 * @brief Vérifie si la réponse choisie est correcte.
 * @param e Structure Enigme.
 * @param reponse Réponse sélectionnée.
 * @return 1 si bonne, sinon 0.
 */
int resoudre_enigme(Enigme* e, int reponse) {
	return reponse == e->bonne_reponse;
}

/**
 * @brief Calcule le nombre de secondes écoulées (limité à TEMPS_MAX).
 * @param debut Temps de départ.
 * @return Durée écoulée.
 */
int gerer_chrono(Uint32 debut) {
	Uint32 maintenant = SDL_GetTicks();
	int frame = (maintenant - debut) / 1000;
	return (frame > TEMPS_MAX) ? TEMPS_MAX : frame;
}

/**
 * @brief Affiche le chronomètre à l'écran selon le temps écoulé.
 * @param e Pointeur vers Enigme.
 * @param ecran Surface SDL.
 */
void afficher_chronometre(Enigme *e, SDL_Surface *ecran) {
	int frame = gerer_chrono(e->debut_chrono);
	frame = (frame > TEMPS_MAX) ? TEMPS_MAX : frame;
	SDL_Rect src_chrono = {frame * 100, 0, 100, 100};
	SDL_BlitSurface(e->chrono_sheet, &src_chrono, ecran, &(SDL_Rect){1200, 20, 100, 100});
}

/**
 * @brief Affiche une question et ses réponses sur l'écran.
 * @param e Structure Enigme.
 * @param ecran Surface SDL.
 */
void afficher_enigme(Enigme *e, SDL_Surface *ecran) {
	SDL_Color blanc = {255, 255, 255};
    
	char question_clean[256];
	strncpy(question_clean, e->question, strcspn(e->question, "\n"));
	question_clean[strcspn(e->question, "\n")] = '\0';

	SDL_Rect pos_question = {200, 200, 0, 0};
	SDL_Surface* texte_question = TTF_RenderText_Blended(e->police, question_clean, blanc);
	SDL_BlitSurface(texte_question, NULL, ecran, &pos_question);
	SDL_FreeSurface(texte_question);

	SDL_Rect pos_reponse = {200, 400, 0, 0};
	for(int i = 0; i < 3; i++) {
		char reponse_clean[256];
		strncpy(reponse_clean, e->reponses[i], strcspn(e->reponses[i], "\n"));
		reponse_clean[strcspn(e->reponses[i], "\n")] = '\0';

		SDL_Surface* texte_reponse = TTF_RenderText_Blended(e->police, reponse_clean, blanc);
		SDL_BlitSurface(texte_reponse, NULL, ecran, &pos_reponse);
		SDL_FreeSurface(texte_reponse);
		pos_reponse.x += 350;
	}
}

/**
 * @brief Applique une animation rotozoom sur une image centrale.
 * @param screen Surface d'affichage.
 * @param image Image source à animer.
 * @param startTime Temps de démarrage.
 * @param currentTime Temps courant.
 */
void afficherEffetRotoZoom(SDL_Surface* screen, SDL_Surface* image, Uint32 startTime, Uint32 currentTime) {
	Uint32 elapsed = currentTime - startTime;
	float progress = (float)elapsed / 3000.0f;
	if(progress > 1.0f) progress = 1.0f;
	double angle = progress * 720.0;
	double zoom = 0.5 + progress * 1.5;
	SDL_Surface* rotated = rotozoomSurface(image, angle, zoom, 1);
	if(rotated) {
		SDL_Rect dest = {
			(screen->w - rotated->w) / 2,
			(screen->h - rotated->h) / 2
	};
	SDL_BlitSurface(rotated, NULL, screen, &dest);
	SDL_FreeSurface(rotated);
	}
}
