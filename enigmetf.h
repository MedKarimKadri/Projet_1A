/**
 * @file enigmetf.h
 * @brief Déclarations des structures et fonctions pour la gestion des énigmes.
 * @author Salma
 * @version 0.1
 * @date 14 mai 2025
 */
#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

/** @def MAX_QUESTIONS
 *  @brief Nombre maximal de questions chargées depuis le fichier.
 */
#define MAX_QUESTIONS 10

/** @def TEMPS_MAX
 *  @brief Durée maximale (en secondes) pour répondre à une question.
 */
#define TEMPS_MAX 12

/**
 * @struct Enigme
 * @brief Contient les données et états pour le jeu d'énigmes.
 */
typedef struct {
	SDL_Surface* chrono_sheet; /**< Sprite contenant les frames du chronomètre. */
	TTF_Font* police;   /**< Police utilisée pour l'affichage des textes. */
	char question[256];  /**< Question actuelle. */
	char reponses[3][256];  /**< Réponses possibles. */
	int bonne_reponse;  /**< Indice de la bonne réponse (0 à 2). */
	int vies;  /**< Nombre de vies restantes. */
	SDL_Surface* victoire;  /**< Image affichée en cas de victoire. */
	SDL_Surface* echec;  /**< Image affichée en cas d'échec. */
	int afficher_echec;  /**< Indicateur d'affichage d'échec. */
	int afficher_victoire;  /**< Indicateur d'affichage de victoire. */
    
	struct {
	char question[256];  /**< Texte de la question. */
        char reponses[3][256];  /**< Réponses associées. */
        int bonne_reponse;  /**< Indice de la bonne réponse. */
	} questions[MAX_QUESTIONS];  /**< Tableau des questions chargées. */
    
	int questions_utilisees[MAX_QUESTIONS];  /**< Marqueur des questions déjà posées. */
	Uint32 debut_chrono;  /**< Temps de début pour le chronomètre. */
	Uint32 effectStartTime;  /**< Temps de démarrage de l'effet visuel. */
	int showingEffect;  /**< Indicateur si un effet est en cours. */
	SDL_Surface* effectImage;  /**< Image à utiliser dans l'effet rotozoom. */
} Enigme;

/**
 * @brief Remplit le fichier de questions avec des données fixes.
 */
void remplir_fichier_questions();
/**
 * @brief Initialise les ressources et données nécessaires pour le jeu d'énigmes.
 * @param e Pointeur vers la structure Enigme.
 * @param ecran Surface d'affichage principale.
 */
void initialiser_enigme(Enigme* e, SDL_Surface* ecran);
/**
 * @brief Sélectionne une question aléatoire non encore utilisée.
 * @param e Pointeur vers la structure Enigme.
 */
void generer_question_aleatoire(Enigme* e);
/**
 * @brief Vérifie si la réponse donnée est correcte.
 * @param e Pointeur vers la structure Enigme.
 * @param reponse Réponse choisie par l'utilisateur (0 à 2).
 * @return 1 si la réponse est correcte, 0 sinon.
 */
int resoudre_enigme(Enigme* e, int reponse);
/**
 * @brief Calcule le temps écoulé depuis le début du chronomètre.
 * @param debut Temps de départ.
 * @return Nombre de secondes écoulées (max TEMPS_MAX).
 */
int gerer_chrono(Uint32 debut);
/**
 * @brief Affiche la question et les réponses à l'écran.
 * @param e Pointeur vers la structure Enigme.
 * @param ecran Surface d'affichage principale.
 */
void afficher_enigme(Enigme *e, SDL_Surface *ecran);
/**
 * @brief Affiche le chronomètre graphique à l'écran.
 * @param e Pointeur vers la structure Enigme.
 * @param ecran Surface d'affichage principale.
 */
void afficher_chronometre(Enigme *e, SDL_Surface *ecran);
/**
 * @brief Applique un effet rotozoom à une image pendant un temps défini.
 * @param screen Surface d'affichage principale.
 * @param image Image à transformer.
 * @param startTime Temps de début de l'effet.
 * @param currentTime Temps actuel.
 */
void afficherEffetRotoZoom(SDL_Surface* screen, SDL_Surface* image, Uint32 startTime, Uint32 currentTime);

#endif
